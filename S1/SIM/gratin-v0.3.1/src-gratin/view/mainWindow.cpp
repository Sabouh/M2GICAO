// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "misc/extinclude.h"
#include "view/mainWindow.h"
#include "view/helpWidget.h"

#include <QDebug>
#include <QMenuBar>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QVBoxLayout>

using namespace std;

const QString MainWindow::_defaultName = "untitled";
QDir MainWindow::_currentPath = QDir::currentPath();

MainWindow::MainWindow()
  : QMainWindow() {

  QSurfaceFormat format = GlContext::surfaceFormat();

   // opengl graph + interaction widgets
   _graphWidget         = new GraphWidget(format);
   _interactionWidget   = new GlInteractionWidget(format);
   
   QWidget *graphEmbed = QWidget::createWindowContainer(_graphWidget);
   QWidget *interactEmbed = QWidget::createWindowContainer(_interactionWidget);
   graphEmbed->setFocusPolicy(Qt::StrongFocus);
   interactEmbed->setFocusPolicy(Qt::StrongFocus);
   interactEmbed->setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum));
   graphEmbed->setFocus();

   // singleton widgets 
   _nodeListWidget      = NodeListWidget::instance(_graphWidget);
   _nodeInterfaceWidget = NodeInterfaceWidget::instance(_graphWidget);
   _animationWidget     = AnimationWidget::instance(_graphWidget);
   _logWidget           = LogWidget::instance();

   createActions();
   createMenus();
  resize(1024,768);

   _interlogWidget = new QDockWidget("View / Interact");
   _interlogWidget->setWidget(interactEmbed);

   setCentralWidget(graphEmbed);
   addDockWidget(Qt::BottomDockWidgetArea,_animationWidget);
   addDockWidget(Qt::LeftDockWidgetArea,_nodeInterfaceWidget);
   addDockWidget(Qt::TopDockWidgetArea,_nodeListWidget);
   addDockWidget(Qt::TopDockWidgetArea,_interlogWidget);

   _animationWidget->setHidden(true);
   _nodeListWidget->setHidden(false);
   _nodeInterfaceWidget->setHidden(true);
   _interlogWidget->setHidden(false);

   _graphWidget->setNodeListWidget(_nodeListWidget);
   _graphWidget->setNodeInterfaceWidget(_nodeInterfaceWidget);
   _graphWidget->setAnimationWidget(_animationWidget);
   _graphWidget->setInteractionWidget(_interactionWidget);
   _graphWidget->setDockInteractionWidget(_interlogWidget);

   _graphWidget->setObjectName("GraphWidget");
   _nodeListWidget->setObjectName("NodeListWidget");
   _nodeInterfaceWidget->setObjectName("NodeInterfaceWidget");
   _interactionWidget->setObjectName("InteractionWidget");
   _animationWidget->setObjectName("AnimationWidget");
   _logWidget->setObjectName("LogWidget");
   _interlogWidget->setObjectName("InterlogWidget");

   setCurrentFile("");

   QString version = QString::number(GRATIN_MAJOR_VERSION)+"."+QString::number(GRATIN_MINOR_VERSION);
   LogWidget::instance()->setText("Welcome to "+Tags::rootTag()+" version "+" "+version+" -- have fun!");
}

MainWindow::~MainWindow() {
  delete _graphWidget;
  delete _nodeListWidget;
  delete _nodeInterfaceWidget;
  delete _animationWidget;
  delete _interactionWidget;
  delete _logWidget;
  delete _interlogWidget;
  delete HelpWidget::instance();
}

void MainWindow::createActions() {
  _newAct = new QAction(tr("&New"), this);
  _newAct->setShortcuts(QKeySequence::New);
  _newAct->setStatusTip(tr("Create a new file"));
  connect(_newAct,SIGNAL(triggered()),this,SLOT(newFile()));

  _openAct = new QAction(tr("&Open..."), this);
  _openAct->setShortcuts(QKeySequence::Open);
  _openAct->setStatusTip(tr("Open an existing file"));
  connect(_openAct,SIGNAL(triggered()),this,SLOT(open()));

  _saveAct = new QAction(tr("&Save"), this);
  _saveAct->setShortcuts(QKeySequence::Save);
  _saveAct->setStatusTip(tr("Save the document to disk"));
  connect(_saveAct,SIGNAL(triggered()),this,SLOT(save()));

  _saveAsAct = new QAction(tr("Save &As..."), this);
  _saveAsAct->setShortcuts(QKeySequence::SaveAs);
  _saveAsAct->setStatusTip(tr("Save the document under a new name"));
  connect(_saveAsAct,SIGNAL(triggered()),this,SLOT(saveAs()));

  _exitAct = new QAction(tr("E&xit"), this);
  _exitAct->setShortcuts(QKeySequence::Quit);
  _exitAct->setStatusTip(tr("Exit the application"));
  connect(_exitAct,SIGNAL(triggered()),this,SLOT(closeAppli()));

  _copyAct = new QAction(tr("Copy"), this);
  _copyAct->setShortcut(QKeySequence::Copy);
  _copyAct->setStatusTip(tr("Copy selection"));
  connect(_copyAct,SIGNAL(triggered()),this,SLOT(copy()));

  _pasteAct = new QAction(tr("Paste"), this);
  _pasteAct->setShortcut(QKeySequence::Paste);
  _pasteAct->setStatusTip(tr("Paste selection"));
  connect(_pasteAct,SIGNAL(triggered()),this,SLOT(paste()));

  _reloadAct = new QAction(tr("Reload"), this);
  _reloadAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
  _reloadAct->setStatusTip(tr("Call the reload function of each node"));
  connect(_reloadAct,SIGNAL(triggered()),this,SLOT(reload()));

  _selectAllAct = new QAction(tr("Select all"), this);
  _selectAllAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
  _selectAllAct->setStatusTip(tr("Select/unselect all nodes"));
  connect(_selectAllAct,SIGNAL(triggered()),this,SLOT(selectAll()));

#ifdef __APPLE__
  // Workaround on Mac to avoid Qt bug (copy, paste, selectAll disabled) when opening QFileDialog
  // https://forum.qt.io/topic/51950/qt-mac-osx-few-menu-items-are-disabled-after-qfiledialog
  _copyAct->setMenuRole(QAction::NoRole);
  _pasteAct->setMenuRole(QAction::NoRole);
  _selectAllAct->setMenuRole(QAction::NoRole);
#endif

  _helpAct = new QAction(tr("&Help"), this);
  _helpAct->setStatusTip(tr("Show the application's Help"));
  connect(_helpAct,SIGNAL(triggered()),this,SLOT(help()));

  _aboutAct = new QAction(tr("&About"), this);
  _aboutAct->setStatusTip(tr("Show the application's About box"));
  connect(_aboutAct,SIGNAL(triggered()),this,SLOT(about()));

  Qt::Modifier frameNavModifier;
#ifdef __APPLE__
  // specific shortcut on OS X
  frameNavModifier = Qt::CTRL;
#else
  frameNavModifier = Qt::SHIFT;
#endif

  _nextFrameAct = new QAction(tr("Next Frame"), this);
  _nextFrameAct->setShortcut(QKeySequence(frameNavModifier + Qt::Key_Right));
  _nextFrameAct->setStatusTip(tr("Increment current frame"));
  connect(_nextFrameAct,SIGNAL(triggered()),this,SLOT(nextFrame()));

  _prevFrameAct = new QAction(tr("Prev Frame"), this);
  _prevFrameAct->setShortcut(QKeySequence(frameNavModifier + Qt::Key_Left));
  _prevFrameAct->setStatusTip(tr("Decrement current frame"));
  connect(_prevFrameAct,SIGNAL(triggered()),this,SLOT(prevFrame()));

  _firstFrameAct = new QAction(tr("First Frame"), this);
  _firstFrameAct->setShortcut(QKeySequence(frameNavModifier + Qt::Key_Up));
  _firstFrameAct->setStatusTip(tr("Cursor at the first frame"));
  connect(_firstFrameAct,SIGNAL(triggered()),this,SLOT(firstFrame()));

  _lastFrameAct = new QAction(tr("Last Frame"), this);
  _lastFrameAct->setShortcut(QKeySequence(frameNavModifier + Qt::Key_Down));
  _lastFrameAct->setStatusTip(tr("Cursor at the last frame"));
  connect(_lastFrameAct,SIGNAL(triggered()),this,SLOT(lastFrame()));

  _playAct = new QAction(tr("Play"), this);
  _playAct->setShortcut(QKeySequence(Qt::Key_P));
  _playAct->setStatusTip(tr("Start animation"));
  connect(_playAct,SIGNAL(triggered()),this,SLOT(play()));

  _stopAct = new QAction(tr("Stop"), this);
  _stopAct->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_P));
  _stopAct->setStatusTip(tr("Stop animation"));
  connect(_stopAct,SIGNAL(triggered()),this,SLOT(stop()));

  _groupAct = new QAction(tr("Group"), this);
  _groupAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
  _groupAct->setStatusTip(tr("Group the current selected graph into a single node"));
  connect(_groupAct,SIGNAL(triggered()),this,SLOT(group()));

  _ungroupAct = new QAction(tr("Ungroup"), this);
  _ungroupAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_G));
  _ungroupAct->setStatusTip(tr("Ungroup the current selected node"));
  connect(_ungroupAct,SIGNAL(triggered()),this,SLOT(ungroup()));

  _addCustomNodeToListAct = new QAction(tr("Add node to list..."), this);
  _addCustomNodeToListAct->setStatusTip(tr("Create an entry in the node list for the current node"));
  connect(_addCustomNodeToListAct,SIGNAL(triggered()),this,SLOT(addNodeToList()));

  _manageNodePathsAct = new QAction(tr("Manage node paths..."), this);
  _manageNodePathsAct->setStatusTip(tr("Add / remove directories in which Gratin will look for nodes"));
  connect(_manageNodePathsAct,SIGNAL(triggered()),this,SLOT(manageNodePaths()));

  _saveContentAct = new QAction(tr("Export node output"), this);
  _saveContentAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));
  _saveContentAct->setStatusTip(tr("Store the selected node output content on disk"));
  connect(_saveContentAct,SIGNAL(triggered()),this,SLOT(saveNodeContent()));

  _saveAnimContentAct = new QAction(tr("Export node output animation"), this);
  _saveAnimContentAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_W));
  _saveAnimContentAct->setStatusTip(tr("Store the selected node output animation content on disk"));
  connect(_saveAnimContentAct,SIGNAL(triggered()),this,SLOT(saveAnimNodeContent()));

  _settingsAct = new QAction(tr("Anim. Settings"), this);
  _settingsAct->setStatusTip(tr("Set range and framerate"));
  connect(_settingsAct,SIGNAL(triggered()),this,SLOT(animSettings()));

  _displayNodeListAct = new QAction(tr("node list window"), this);
  connect(_displayNodeListAct,SIGNAL(triggered()),this,SLOT(displayNodeList()));

  _displayInteractionAct = new QAction(tr("interaction window"), this);
  connect(_displayInteractionAct,SIGNAL(triggered()),this,SLOT(displayInteraction()));

  _displayAnimationAct = new QAction(tr("animation window"), this);
  connect(_displayAnimationAct,SIGNAL(triggered()),this,SLOT(displayAnimation()));

  _displayNodeInterfaceAct = new QAction(tr("interface window"), this);
  connect(_displayNodeInterfaceAct,SIGNAL(triggered()),this,SLOT(displayNodeInterface()));

  _interactZoomInAct = new QAction(tr("Interact (zoom in)"), this);
  _interactZoomInAct->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Plus));
  connect(_interactZoomInAct,SIGNAL(triggered()),this,SLOT(interactZoomIn()));

  _interactZoomOutAct = new QAction(tr("Interact (zoom out)"), this);
  _interactZoomOutAct->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Minus));
  connect(_interactZoomOutAct,SIGNAL(triggered()),this,SLOT(interactZoomOut()));

  _interactNoZoomAct = new QAction(tr("Interact (reset)"), this);
  connect(_interactNoZoomAct,SIGNAL(triggered()),this,SLOT(interactNoZoom()));

  _interactViewAllAct = new QAction(tr("Interact (view all)"), this);
  connect(_interactViewAllAct,SIGNAL(triggered()),this,SLOT(interactViewAll()));

  _graphZoomInAct = new QAction(tr("Graph (zoom in)"), this);
  _graphZoomInAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus));
  connect(_graphZoomInAct,SIGNAL(triggered()),this,SLOT(graphZoomIn()));

  _graphZoomOutAct = new QAction(tr("Graph (zoom out)"), this);
  _graphZoomOutAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus));
  connect(_graphZoomOutAct,SIGNAL(triggered()),this,SLOT(graphZoomOut()));

  _graphNoZoomAct = new QAction(tr("Graph (reset)"), this);
  connect(_graphNoZoomAct,SIGNAL(triggered()),this,SLOT(graphNoZoom()));

  _graphViewAllAct = new QAction(tr("Graph (view all)"), this);
  connect(_graphViewAllAct,SIGNAL(triggered()),this,SLOT(graphViewAll()));
}

void MainWindow::createMenus() {
  _fileMenu = menuBar()->addMenu(tr("&File"));
  _fileMenu->addAction(_newAct);
  _fileMenu->addAction(_openAct);
  _fileMenu->addAction(_saveAct);
  _fileMenu->addAction(_saveAsAct);
  _fileMenu->addSeparator();
  _fileMenu->addAction(_exitAct);
  
  _editMenu = menuBar()->addMenu(tr("&Edit"));
  _editMenu->addAction(_copyAct);
  _editMenu->addAction(_pasteAct);
  _editMenu->addAction(_selectAllAct);
  _editMenu->addAction(_reloadAct);

  _animMenu = menuBar()->addMenu(tr("&Animate"));
  _animMenu->addAction(_playAct);
  _animMenu->addAction(_stopAct);
  _animMenu->addSeparator();
  _animMenu->addAction(_nextFrameAct);
  _animMenu->addAction(_prevFrameAct);
  _animMenu->addAction(_firstFrameAct);
  _animMenu->addAction(_lastFrameAct);
  _animMenu->addSeparator();
  _animMenu->addAction(_settingsAct);
  
  _toolsMenu = menuBar()->addMenu(tr("&Tools"));
  _toolsMenu->addAction(_groupAct);
  _toolsMenu->addAction(_ungroupAct);
  _toolsMenu->addSeparator();
  _toolsMenu->addAction(_saveContentAct);
  _toolsMenu->addAction(_saveAnimContentAct);
  _toolsMenu->addSeparator();
  _toolsMenu->addAction(_addCustomNodeToListAct);
  _toolsMenu->addAction(_manageNodePathsAct);

  _windowMenu = menuBar()->addMenu(tr("&Window"));
  _showHideMenu = _windowMenu->addMenu(tr("Show/Hide"));
  _showHideMenu->addAction(_displayNodeListAct);
  _showHideMenu->addAction(_displayInteractionAct);
  _showHideMenu->addAction(_displayAnimationAct);
  _showHideMenu->addAction(_displayNodeInterfaceAct);
  _zoomInOutMenu = _windowMenu->addMenu(tr("Zooms"));
  _zoomInOutMenu->addAction(_graphZoomInAct);
  _zoomInOutMenu->addAction(_graphZoomOutAct);
  _zoomInOutMenu->addAction(_graphNoZoomAct);
  _zoomInOutMenu->addAction(_graphViewAllAct);
  _zoomInOutMenu->addAction(_interactZoomInAct);
  _zoomInOutMenu->addAction(_interactZoomOutAct);
  _zoomInOutMenu->addAction(_interactNoZoomAct);
  _zoomInOutMenu->addAction(_interactViewAllAct);

  _helpMenu = menuBar()->addMenu(tr("&Help"));
  _helpMenu->addAction(_helpAct);
  _helpMenu->addAction(_aboutAct);

  QAction *act;
  act = menuBar()->addAction("");
  act->setIcon(QIcon(":/horizontal-dashed"));
  menuBar()->addAction(LogWidget::instance());
  act = menuBar()->addAction("");
  act->setIcon(QIcon(":/horizontal-dashed"));
 }

void MainWindow::newFile() {
  if(maybeSave()) {
    clearAll();
    setCurrentFile("");
  }
}

void MainWindow::open() {
  if(maybeSave()) {
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Gratin File"),_currentPath.absolutePath(),
						    QString("Gratin Files (*")+GRATIN_EXT+QString(")"));
    if(!fileName.isEmpty()) {
      clearAll();
      loadFile(fileName);

      QDir d(".");
      _currentPath = d.filePath(fileName);
    }
  }
}

bool MainWindow::save() {
  if(_currentFile.isEmpty()) {
    return saveAs();
  } else {
    return saveFile(_currentFile);
  }
}

bool MainWindow::saveAs() {
  QString fileName = QFileDialog::getSaveFileName(this,tr("Save Gratin File"),_currentPath.absolutePath(),
						  QString("Gratin Files (*")+GRATIN_EXT+QString(")"));
  if(fileName.isEmpty())
    return false;

  if(!fileName.endsWith(GRATIN_EXT)) 
    fileName.append(GRATIN_EXT);

  QDir d(".");
  _currentPath = d.filePath(fileName);

  return saveFile(fileName);
}

void MainWindow::reload() {
  _graphWidget->reloadShaders();
}

void MainWindow::copy() {
  _graphWidget->copy();
}

void MainWindow::paste() {
  _graphWidget->paste();
}

void MainWindow::selectAll() {
  _graphWidget->selectUnselectAll();
}

void MainWindow::help() {
  if(HelpWidget::instance()->empty()) {
    initHelpWidget();
  }

  HelpWidget::instance()->show();
}

void MainWindow::about() {
  QString h = tr("<center><font size='12'> Gratin version 0.3</font></center><br>"
		 "a programmable Node-based System for GPU-friendly Applications.<br><br>"
		 "Copyright (C) 2013-2015: <br>"
		 "<a href='http://maverick.inria.fr/~Romain.Vergne/'>Romain Vergne</a> "
		 "( <a href='mailto:romain.vergne@inria.fr'>romain.vergne@inria.fr</a> ): spec/core-dev <br>"
		 "<a href='www.labri.fr/perso/barla/'>Pascal Barla</a> "
		 "( <a href='mailto:>pascal.barla@inria.fr'>pascal.barla@inria.fr</a> ): spec <br><br>"
		 "Please, visit <a href='http://gratin.gforge.inria.fr/'>our page</a> for more information.<br><br>"
		 "This program is subject to the terms of the<br>"
		 "<a href='http://mozilla.org/MPL/2.0/'>Mozilla Public License v. 2.0.</a><br><br>"
		 //"Special thanks to:<br>"
		 //"Daniel Holtmann-Rice (for testing MacOSX, OpenGL versioning)<br>"
		 //"Steven A. Cholewiak (for testing MacOSX)<br>"
		 //"Pierre Bénard (for improving Hermite curves)<br>"
		 );
		 
  QMessageBox::about(this,"About",h);
}

bool MainWindow::closeAppli() {
  return close();
}

void MainWindow::nextFrame() {
  _graphWidget->animation()->incrementCurrentFrame();
  _animationWidget->animationFrameChanged();
}

void MainWindow::prevFrame() {
  _graphWidget->animation()->decrementCurrentFrame();
  _animationWidget->animationFrameChanged();
}

void MainWindow::firstFrame() {
  _graphWidget->animation()->goToFirst();
  _animationWidget->animationFrameChanged();
}

void MainWindow::lastFrame() {
  _graphWidget->animation()->goToLast();
  _animationWidget->animationFrameChanged();
}

void MainWindow::play() {
  _animationWidget->play();
}

void MainWindow::stop() {
  _animationWidget->stop();
}

void MainWindow::animSettings() {
  AnimationSettings *w = new AnimationSettings(_graphWidget);
  w->show();
}

void MainWindow::displayNodeList() {
  if(_nodeListWidget->isHidden())
    _nodeListWidget->show();
  else
    _nodeListWidget->hide();
}

void MainWindow::displayInteraction() {
  if(_interlogWidget->isHidden())
    _interlogWidget->show();
  else
    _interlogWidget->hide();
}

void MainWindow::displayAnimation() {
  if(_animationWidget->isHidden())
    _animationWidget->show();
  else
    _animationWidget->hide();
}

void MainWindow::displayNodeInterface() {
  if(_nodeInterfaceWidget->isHidden())
    _nodeInterfaceWidget->show();
  else
    _nodeInterfaceWidget->hide();
}

void MainWindow::group() {
  _graphWidget->group();
}

void MainWindow::ungroup() {
  _graphWidget->ungroup();
}

void MainWindow::addNodeToList() {
  _graphWidget->addNodeToList();
}

void MainWindow::manageNodePaths() {
  _graphWidget->manageNodePaths();
}

void MainWindow::saveNodeContent() {
  _graphWidget->saveNodeContent();
}

void MainWindow::saveAnimNodeContent() {
  _graphWidget->saveAnimNodeContent();
}

void MainWindow::interactZoomIn() {
  _interactionWidget->zoom(1);
}

void MainWindow::interactZoomOut() {
  _interactionWidget->zoom(-1);
}

void MainWindow::interactNoZoom() {
  _interactionWidget->zoom(0);
}

void MainWindow::interactViewAll() {
  _interactionWidget->viewAll();
}

void MainWindow::graphZoomIn() {
  _graphWidget->zoom(1);
}

void MainWindow::graphZoomOut() {
  _graphWidget->zoom(-1);
}

void MainWindow::graphNoZoom() {
  _graphWidget->zoom(0);
}

void MainWindow::graphViewAll() {
  _graphWidget->viewAll();
}

bool MainWindow::maybeSave() {
  if(!_graphWidget->graphIsEmpty()) {
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this,tr("Question"),tr("Do you want to save your scene?"),
			       QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if(ret==QMessageBox::Save)
      return save();
    else if(ret==QMessageBox::Cancel)
      return false;
  }
  return true;
}

void MainWindow::clearAll() {
  _graphWidget->clear();
}

void MainWindow::closeEvent(QCloseEvent *event) {
  if(maybeSave()) {
    clearAll();
    event->accept();
  } else {
    event->ignore();
  }
}

void MainWindow::setCurrentFile(const QString &fileName) {
  _currentFile = fileName;
  QString shownName = _currentFile;
  if(_currentFile.isEmpty())
    shownName = _defaultName+GRATIN_EXT;
  setWindowFilePath(shownName);
}

bool MainWindow::loadFile(const QString &fileName) {
  QFile file(fileName);
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("Error!"),
			 tr("Cannot read file %1:\n%2.")
			 .arg(fileName)
			 .arg(file.errorString()));
    return false;
  }

  QFileInfo info(fileName);
  bool success = true;

  if(!_loader.readFrom(file))                       success = false;
  if(!_loader.init("pipeline",info.absolutePath())) success = false;
  if(!_loader.loadIOData("mainWindow",this))        success = false;

  if(success) {
    LogWidget::instance()->setText(fileName+" successfully loaded!");
  } else {
    LogWidget::instance()->setText("Some errors occurred when loading "+fileName+"...");
  }

  setCurrentFile(fileName);
  _graphWidget->applyNodes();

  return true;
}

bool MainWindow::saveFile(const QString &fileName) {
  QFile file(fileName);
  if(!file.open(QFile::WriteOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("Error!"),
			 tr("Cannot write file %1:\n%2.")
			 .arg(fileName)
			 .arg(file.errorString()));
    return false;
  }

  QFileInfo info(fileName);

  bool b1 = _saver.init("pipeline",info.absolutePath());
  bool b2 = _saver.saveIOData("mainWindow",this);

  if(_saver.writeTo(file) && b1 && b2) {
    LogWidget::instance()->setText(fileName+" successfully saved!");
  } else {
    LogWidget::instance()->setText("Some errors occurred when saving "+fileName+"...");
  }

  setCurrentFile(fileName);
  return true;
}

bool MainWindow::save(SceneSaver *saver) {
  bool ret = true;

  if(!saver->saveIOData("graphWidget"        ,_graphWidget        )) ret = false;
  if(!saver->saveIOData("animationWidget"    ,_animationWidget    )) ret = false;
  if(!saver->saveIOData("nodeListWidget"     ,_nodeListWidget     )) ret = false;
  if(!saver->saveIOData("interactionWidget"  ,_interactionWidget  )) ret = false;
  if(!saver->saveIOData("nodeInterfaceWidget",_nodeInterfaceWidget)) ret = false;

  return ret;
}

bool MainWindow::load(SceneLoader *loader) {
  bool ret = true;

  // load values 
  if(!loader->loadIOData("graphWidget"        ,_graphWidget        )) ret = false;
  if(!loader->loadIOData("animationWidget"    ,_animationWidget    )) ret = false;
  if(!loader->loadIOData("nodeListWidget"     ,_nodeListWidget     )) ret = false;
  if(!loader->loadIOData("interactionWidget"  ,_interactionWidget  )) ret = false;
  if(!loader->loadIOData("nodeInterfaceWidget",_nodeInterfaceWidget)) ret = false;

  return ret;
}

void MainWindow::initHelpWidget() {
  vector<NodeHandleInterface *> h = NodeManager::instance()->nodeHandles();
  for(unsigned int i=0;i<h.size();++i) {
    HelpWidget::instance()->add(h[i]->name(),h[i]->help());
  }
}
