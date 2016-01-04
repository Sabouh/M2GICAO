// This file is part of Gratin, a programmable Node-based System
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "GLSLoader.h"
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QFileDialog>
#include <iostream>
#include <stack>
#include <queue>

#include "misc/extinclude.h"
#include "misc/glutils.h"
#include "core/pbgraph.h"
#include "misc/mesh.h"
#include "misc/objLoader.h"

using namespace std;

QDir GLSLoaderWidget::_currentPath = QDir::currentPath();

GLSLoaderWidget::GLSLoaderWidget(GLSLoaderNode *node)
  : GenericCustomWidget(node),
    _load(new QPushButton("Load...")),
    _compute(new QPushButton("Re-compute")),
    _default(new QPushButton("Reset cam")),
    _radius(new FloatSliderWidget(node,"Radius",0.f,1.f,0.15f)) {

    _updateRadius = new QCheckBox("Auto-update radius on load");
    _updateRadius->setChecked(true);
    _curvMinMax = new Vector2fSpinWidget(node,"Min/max curvature",Vector2f(-10000,-10000),Vector2f(10000,10000),Vector2f(-1,1));
    _updateMinMax = new QCheckBox("Auto-update min/max curvature after fitting");
    _updateMinMax->setChecked(true);
    _camWidget = new TrackballCameraWidget(node,"Camera",node->camera());

  // default parameters 
  userLayout()->addWidget(_load);
  userLayout()->addWidget(_updateRadius);
  userLayout()->addWidget(_radius);
  userLayout()->addWidget(_curvMinMax);
  userLayout()->addWidget(_updateMinMax);
  userLayout()->addWidget(_compute);
  userLayout()->addWidget(_camWidget);
  userLayout()->addWidget(_default);

  connect(_load,SIGNAL(clicked()),this,SLOT(loadClicked()));
  connect(_default,SIGNAL(clicked()),this,SLOT(defaultClicked()));
  connect(_compute,SIGNAL(clicked()),this,SLOT(computeClicked()));

  addChildWidget(_camWidget);
}

void GLSLoaderWidget::loadClicked() {
  QString filename = QFileDialog::getOpenFileName(0,"Load object",_currentPath.absolutePath(),"Objects (*.obj *.off *.stl);;All (*.*)");

  if(filename.isEmpty()) {
    return;
  }

  QDir d(".");
  _currentPath = d.filePath(filename);

  ((GLSLoaderNode *)node())->loadObject(filename);
  updateGraph();
}

void GLSLoaderWidget::defaultClicked() {
  ((GLSLoaderNode *)node())->initCam();
  updateGraph();
}


void GLSLoaderWidget::computeClicked() {
    ((GLSLoaderNode *)node())->fit();
    updateGraph();
}


GLSLoaderNode::GLSLoaderNode(PbGraph *parent,NodeHandle *handle)
  : GenericCustomNode(true,false,false,true,parent,handle,true,false,Vector2f(0.0f,0.0f),Vector2f(512.0f,512.0f)),
    _filename(GRATIN_APPLI_DATA_PATH+"/objs/sphere.obj"),
    _camera(new TrackballCamera(Vector2i(512,512),Vector3f(0,0,0),1.0f,0)),
    _vao(NULL),
    _depth(NULL) {

  _w = new GLSLoaderWidget(this);

  initShaderSource();
  loadObject(_filename);

  _w->updateWidget();
}

GLSLoaderNode::~GLSLoaderNode() {
  delete _vao;
  delete _camera;
  delete _depth;
}

void GLSLoaderNode::apply() {
  //CHECK_GL_ERROR
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());
  setOutputParams();

  _fbo.bind();
  _glf->glDrawBuffers(nbOutputs(),buffersOfOutputTex(0));

  initOpenGLState();

  enableShaders();
  _p->setUniformMatrix4fv("model",(GLfloat *)_camera->modelMatrix().data());
  _p->setUniformMatrix4fv("view",(GLfloat *)_camera->viewMatrix().data());
  _p->setUniformMatrix4fv("proj",(GLfloat *)_camera->projMatrix().data());
  _p->setUniform1f("zmin",_camera->zmin());
  _p->setUniform1f("zmax",_camera->zmax());
  _p->setUniform2fv("curvMinMax",(GLfloat *)_w->curvMinMax().data());

  _vao->bind();

  if(useTesselation()) {
    _glf->glPatchParameteri(GL_PATCH_VERTICES,3);
    _vao->drawElements(GL_PATCHES,_nbElements);
  } else {
    _vao->drawElements(GL_TRIANGLES,_nbElements);
  }

  _vao->unbind();
  FramebufferObject::unbind();

  disableShaders();
  cleanOpenGLState();
  //CHECK_GL_ERROR
}

void GLSLoaderNode::loadObject(const QString &filename) {
  QString f = filename;
  QDir d(".");
  f = d.absoluteFilePath(f);

  if(!_mesh.read(f.toStdString()))
      return;

  _filename = f;

  // compute barycenter and radius
  Surface_mesh::Vertex_property<Point> vertices = _mesh.get_vertex_property<Point>("v:point");
  Surface_mesh::Vertex_iterator vit, vend = _mesh.vertices_end();

  // compute barycenter and radius
  _meshBarycenter = Vector3f::Zero();
  for (vit = _mesh.vertices_begin(); vit != vend; ++vit)
  {
      _meshBarycenter += vertices[*vit];
  }
  _meshBarycenter /= _mesh.n_vertices();
  _meshRadius = 0.f;
  for (vit = _mesh.vertices_begin(); vit != vend; ++vit)
  {
      float dist = (vertices[*vit]-_meshBarycenter).norm();
      if(dist > _meshRadius)
          _meshRadius = dist;
  }

  // init camera
  _camera->setSceneParams(_meshBarycenter,_meshRadius);

  if(!_mesh.is_triangle_mesh())
      _mesh.triangulate();
  _mesh.update_vertex_normals();
  Surface_mesh::Vertex_property<Point> vnormals = _mesh.get_vertex_property<Point>("v:normal");

  // init VAO
  delete _vao;
  _vao = new VertexarrayObject();
  _vao->addAttrib(_mesh.n_vertices()*sizeof(Vector3f), vertices.data(), 3);
  _vao->addAttrib(_mesh.n_vertices()*sizeof(Vector3f), vnormals.data(), 3);

  if(_w->_updateRadius->isChecked())
      _w->_radius->setVal(findMinRadius(),false);
  fit(true);

  // get face indices
  std::vector<int> faces;
  faces.reserve(_mesh.n_faces()*3);
  Surface_mesh::Face_iterator fit, fend = _mesh.faces_end();
  Surface_mesh::Vertex_around_face_circulator fvit, fvend;
  for (fit = _mesh.faces_begin(); fit != fend; ++fit)
  {
      fvit = _mesh.vertices(*fit);
      fvend = fvit;

      do{
          faces.push_back((*fvit).idx());
      } while (++fvit != fvend);
  }
      _vao->setIndices(_mesh.n_faces()*sizeof(Vector3i), &faces[0]);

  _nbElements = _mesh.n_faces()*3;
}

void GLSLoaderNode::fit(bool init) {
    Surface_mesh::Vertex_property<Point> vertices = _mesh.get_vertex_property<Point>("v:point");
    Surface_mesh::Vertex_property<Point> vnormals = _mesh.get_vertex_property<Point>("v:normal");

    // compute curvature
    std::vector<Vector3f> maxDirections;
    std::vector<Vector3f> minDirections;
    maxDirections.resize(_mesh.n_vertices());
    minDirections.resize(_mesh.n_vertices());
    std::vector<float> maxCurvature;
    std::vector<float> minCurvature;
    std::vector<float> geomVar;
    maxCurvature.resize(_mesh.n_vertices());
    minCurvature.resize(_mesh.n_vertices());
    geomVar.resize(_mesh.n_vertices());

    Surface_mesh::Vertex v0, v1;
    Surface_mesh::Vertex_around_vertex_circulator vc, vc_end;

    std::queue<Surface_mesh::Vertex> nrings;

    float r = 2.f * _meshRadius * _w->radius();
    float gCurvMax = 0.f;

    int i=0;
    for (Surface_mesh::Vertex_iterator vit = _mesh.vertices_begin(); vit != _mesh.vertices_end(); ++vit)
    {
        v0 = *vit;

        // initialize fit
        Fit fit;
        fit.init(vertices[v0]);
        nrings.push(v0);

        fit.setWeightFunc(WeightFunc(r));

        int nbNeighboors = 0;

        // process stack
        if(_mesh.halfedge(v0).is_valid()) {
            Surface_mesh::Vertex_property<bool> processed = _mesh.add_vertex_property<bool>("v:processed",false);
            processed[v0] = true;
            while(!nrings.empty()) {
                v1 = nrings.front();
                nrings.pop();
                // add to the fit
                fit.addNeighbor(GLSPoint(vertices[v1], vnormals[v1]));
                nbNeighboors++;

                // add its 1-ring neighboors to the stack if they haven't been processed yet...
                vc = _mesh.vertices(v1);
                vc_end = vc;
                do {
                    if(!processed[*vc]) {
                        processed[*vc] = true;
                        // ...and if they are close enough
                        if((vertices[*vc] - vertices[v0]).norm() < r)
                            nrings.push(*vc);
                    }
                } while (++vc != vc_end);
            }
            _mesh.remove_vertex_property<bool>(processed);
        }

        fit.finalize();

        if(fit.isReady()) {
            if(!fit.isStable())
                qWarning() << "Unstable fit: " << nbNeighboors << " neighbhoors";

            maxDirections[i] = fit.GLSk1Direction();
            minDirections[i] = fit.GLSk2Direction();
            maxCurvature[i]  = fit.GLSk1();
            minCurvature[i]  = fit.GLSk2();
            geomVar[i]       = fit.geomVar();

            float gCurv = fabs(fit.GLSGaussianCurvature());
            if(gCurv > gCurvMax)
                gCurvMax = gCurv;
        } else {
            qCritical() << "Fit impossible: " << nbNeighboors << " neighbhoors";
            maxDirections[i] = Vector3f::Zero();
            minDirections[i] = Vector3f::Zero();
            maxCurvature[i]  = std::numeric_limits<float>::quiet_NaN();
            minCurvature[i]  = std::numeric_limits<float>::quiet_NaN();
            geomVar[i]       = std::numeric_limits<float>::quiet_NaN();
        }
        i++;
    }

    if(_w->_updateMinMax->isChecked())
        _w->_curvMinMax->setVal(Vector2f(-gCurvMax,gCurvMax),false);

    if(init) {
        _vao->addAttrib(_mesh.n_vertices()*sizeof(Vector3f), &maxDirections[0], 3);
        _vao->addAttrib(_mesh.n_vertices()*sizeof(Vector3f), &minDirections[0], 3);
        _vao->addAttrib(_mesh.n_vertices()*sizeof(float), &maxCurvature[0], 1);
        _vao->addAttrib(_mesh.n_vertices()*sizeof(float) ,&minCurvature[0], 1);
        _vao->addAttrib(_mesh.n_vertices()*sizeof(float) ,&geomVar[0], 1);
    } else {
        _vao->setAttrib(2,_mesh.n_vertices()*sizeof(Vector3f), &maxDirections[0], 3);
        _vao->setAttrib(3,_mesh.n_vertices()*sizeof(Vector3f), &minDirections[0], 3);
        _vao->setAttrib(4,_mesh.n_vertices()*sizeof(float), &maxCurvature[0], 1);
        _vao->setAttrib(5,_mesh.n_vertices()*sizeof(float) ,&minCurvature[0], 1);
        _vao->setAttrib(6,_mesh.n_vertices()*sizeof(float) ,&geomVar[0], 1);
    }
}

float GLSLoaderNode::findMinRadius() {
    Surface_mesh::Vertex_property<Point> vertices = _mesh.get_vertex_property<Point>("v:point");
    Surface_mesh::Vertex_around_vertex_circulator vc, vc_end;
    Surface_mesh::Vertex v0, v1;

    float minRadius = 0.f;

    for (Surface_mesh::Vertex_iterator vit = _mesh.vertices_begin(); vit != _mesh.vertices_end(); ++vit)
    {
        v0 = *vit;
        if(!_mesh.halfedge(v0).is_valid()) {
            continue;
        }
        std::queue<Surface_mesh::Vertex> nrings;
        nrings.push(v0);

        int nbNeighboors = 0;

        Surface_mesh::Vertex_property<bool> processed = _mesh.add_vertex_property<bool>("v:processed",false);
        processed[v0] = true;
        while(!nrings.empty() && nbNeighboors < 6) {
            v1 = nrings.front();
            nrings.pop();
            float dist = (vertices[v1] - vertices[v0]).norm();
            if(dist > minRadius) {
                minRadius = dist;
            }
            nbNeighboors++;

            vc = _mesh.vertices(v1);
            vc_end = vc;
            do {
                if(!processed[*vc]) {
                    processed[*vc] = true;
                    nrings.push(*vc);
                }
            } while (++vc != vc_end);
        }
        _mesh.remove_vertex_property<bool>(processed);

    }
    return minRadius / (2.f * _meshRadius) + 0.001;
}

void GLSLoaderNode::initCam() {
  Vector2i size(outputSize()[0],outputSize()[1]);
  _camera->reinit(size,_camera->sceneCenter(),_camera->sceneRadius());
}

void GLSLoaderNode::mousePressEvent(const Vector2f &p,QMouseEvent *me) {
  if(me->button()==Qt::LeftButton) {
    _camera->initRotation(p);
  } else if(me->button()==Qt::RightButton) {
    _camera->initMoveXY(p);
  } else if(me->button()==Qt::MidButton) {
    _camera->initMoveZ(p);
  }

  GenericCustomNode::mousePressEvent(p,me);
}

void GLSLoaderNode::mouseMoveEvent(const Vector2f &p,QMouseEvent *me) {
  _camera->move(p);
  GenericCustomNode::mouseMoveEvent(p,me);
}

void GLSLoaderNode::wheelEvent(const Vector2f &p,QWheelEvent *we) {
    GenericCustomNode::wheelEvent(p,we);

    const Vector2f v(0.0f,60.0f);

    _camera->initMoveZ(p);
    if(we->delta()>0) {
        _camera->move(p+v);
    } else {
        _camera->move(p-v);
    }

    update();
}

void GLSLoaderNode::initFBO() {
  NodeTexture2D::initFBO();

  _depth = new FloatTexture2D(TextureFormat(GL_TEXTURE_2D,outputSize()[0],outputSize()[1],
  					    GL_DEPTH_COMPONENT24,GL_DEPTH_COMPONENT,GL_FLOAT),
  			      TextureParams(GL_NEAREST,GL_NEAREST));

  _fbo.bind();
  _fbo.attachTexture(GL_TEXTURE_2D,_depth->id(),GL_DEPTH_ATTACHMENT);
  _fbo.isValid();

  FramebufferObject::unbind();
}

void GLSLoaderNode::cleanFBO() {
  NodeTexture2D::cleanFBO();
  delete _depth; _depth = NULL;
}

const QString GLSLoaderNode::constantVertHead()  {
  return QObject::tr("layout(location = 0) in vec3 inVertex;\n"
                     "layout(location = 1) in vec3 inNormal;\n"
                     "layout(location = 2) in vec3 inMaxDirection;\n"
                     "layout(location = 3) in vec3 inMinDirection;\n"
                     "layout(location = 4) in float inMaxCurvature;\n"
                     "layout(location = 5) in float inMinCurvature;\n"
                     "layout(location = 6) in float inGeomVar;\n"
                     );
}

const QString GLSLoaderNode::defaultVertBody()   {
  return QObject::tr("out vec3  normalV;\n"
             "out float depthV;\n"
             "out float curvatureV;\n\n"
		     "void main() {\n"
		     "\tmat4 mdv    = view*model;\n"
		     "\tmat4 mvp    = proj*mdv;\n\n"
		     "\tnormalV     = (mdv*vec4(inNormal,0)).xyz;\n"
             "\tcurvatureV  = inMaxCurvature*inMinCurvature; //gaussian curvature\n"
		     "\tdepthV      = (clamp(-(mdv*vec4(inVertex,1)).z,zmin,zmax)-zmin)/(zmax-zmin);\n"
		     "\tgl_Position = mvp*vec4(inVertex,1);\n"
		     "}\n");
}

const QString GLSLoaderNode::constantFragHead()  {
  return QString();
}

const QString GLSLoaderNode::defaultFragBody()   {
  return QObject::tr("in vec3 normalV;\n"
             "in float depthV;\n"
             "in float curvatureV;\n\n"
             "vec3 getColor(float _value, float _valueMin, float _valueMax) {\n"
             "\tvec3 c = vec3(1.0, 1.0, 1.0);\n"
             "\tfloat dv;\n"
             "\tif(_value == 0.)\n"
             "\t\treturn c;\n"
             "\tif(isnan(_value))\n"
             "\t\treturn vec3(0.0, 1.0, 0.0);\n"
             "\tif (_value < _valueMin)\n"
             "\t\t_value = _valueMin;\n"
             "\tif (_value > _valueMax)\n"
             "\t\t_value = _valueMax;\n"
             "\tdv = _valueMax - _valueMin;\n"
             "\tif(_value < (_valueMin + 0.5 * dv)){\n"
             "\t\tc.r = 2 * (_value - _valueMin) / dv;\n"
             "\t\tc.g = 2 * (_value - _valueMin) / dv;\n"
             "\t\tc.b = 1;\n"
             "\t}else{\n"
             "\t\tc.b = 2 - 2 * (_value - _valueMin) / dv;\n"
             "\t\tc.g = 2 - 2 * (_value - _valueMin) / dv;\n"
             "\t\tc.r = 1;\n"
             "\t}\n"
             "\treturn c;\n"
             "}\n\n"
		     "void main() {\n"
             "\toutBuffer0 = vec4(getColor(curvatureV,curvMinMax[0],curvMinMax[1]),1);\n"
		     "}\n");
}

const QString GLSLoaderNode::constantTessCHead() {
  return QObject::tr("layout(vertices = 3) out;\n");
}

const QString GLSLoaderNode::defaultTessCBody()  {
return QObject::tr("void main(void) {\n"
		   "\tgl_TessLevelOuter[0] = 1;\n"
		   "\tgl_TessLevelOuter[1] = 1;\n"
		   "\tgl_TessLevelOuter[2] = 1;\n"
		   "\tgl_TessLevelOuter[3] = 1;\n"
		   "\tgl_TessLevelInner[0] = 1;\n"
		   "\tgl_TessLevelInner[1] = 1;\n"
		   "\tgl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;\n"
		   "}\n");
}

const QString GLSLoaderNode::constantTessEHead() {
  return QString();
}

const QString GLSLoaderNode::defaultTessEBody()  {
  return QObject::tr("layout(triangles,equal_spacing,ccw) in;\n\n"
  		     "void main() {\n"
  		     "\tvec4 p1 = gl_TessCoord.x*gl_in[0].gl_Position;\n"
  		     "\tvec4 p2 = gl_TessCoord.y*gl_in[1].gl_Position;\n"
  		     "\tvec4 p3 = gl_TessCoord.z*gl_in[2].gl_Position;\n"
  		     "\tgl_Position = p1+p2+p3;\n"
  		     "}\n");
}

const QString GLSLoaderNode::constantGeomHead()  {
  return QObject::tr("layout(triangles) in;\n");
}

const QString GLSLoaderNode::defaultGeomBody()   {
    return QObject::tr("layout(triangle_strip, max_vertices = 3) out;\n\n"
               "void main() {\n"
               "\tgl_Position = gl_in[0].gl_Position; EmitVertex();\n"
               "\tgl_Position = gl_in[1].gl_Position; EmitVertex();\n"
               "\tgl_Position = gl_in[2].gl_Position; EmitVertex();\n"
               "\tEndPrimitive();\n"
               "}\n");
}

bool GLSLoaderNode::save(SceneSaver  *saver) {
  bool ret = true;

  // obj file and camera 
  QString name = saver->absoluteToRelativePath(_filename);
  if(!saver->saveString("filename",name))      ret = false;
  if(!saver->saveIOData("camera",_camera))     ret = false;
  if(!GenericCustomNode::save(saver))          ret = false;
  if(!_w->_radius->save(saver))                ret = false;
  if(!_w->_curvMinMax->save(saver))            ret = false;

  return ret;
}

bool GLSLoaderNode::load(SceneLoader *loader) {
  bool ret = true;

  _filename = GRATIN_APPLI_DATA_PATH+"/objs/sphere.obj";
  QString name;
  if(!loader->loadString("filename",name)) ret = false;

  if(ret) {
    _filename = loader->relativeToAbsolutePath(name);
  }

  if(!loader->loadIOData("camera",_camera))     ret = false;
  loadObject(_filename);
  if(!GenericCustomNode::load(loader))          ret = false;

  if(!_w->_radius->load(loader))                ret = false;
  if(!_w->_curvMinMax->load(loader))            ret = false;

  return ret;
}
