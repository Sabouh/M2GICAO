// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "misc/extinclude.h"
#include "core/sceneIOTags.h"
#include "view/graphWidget.h"
#include "view/interactionWidget.h"
#include "view/nodeInterfaceWidget.h"
#include "view/nodeListWidget.h"
#include "view/animationWidget.h"
#include "view/logWidget.h"
#include <QMainWindow>
#include <QString>
#include <QAction>
#include <QMenu>
#include <QDir>


class LIBRARY_EXPORT MainWindow : public QMainWindow, public IOData {
  Q_OBJECT

    public:
  MainWindow();
  ~MainWindow();

  bool save(SceneSaver  *saver );
  bool load(SceneLoader *loader);
  bool loadFile(const QString &fileName);
  bool saveFile(const QString &fileName);

 private:
  GraphWidget         *_graphWidget;
  NodeListWidget      *_nodeListWidget;
  GlInteractionWidget *_interactionWidget;
  NodeInterfaceWidget *_nodeInterfaceWidget;
  AnimationWidget     *_animationWidget;
  LogWidget           *_logWidget;
  QDockWidget         *_interlogWidget;

 protected:
  void closeEvent(QCloseEvent *event);

  private slots:
  void newFile();
  void open();
  bool save();
  bool saveAs();
  void help();
  void about();
  bool closeAppli();
  void nextFrame();
  void prevFrame();
  void firstFrame();
  void lastFrame();
  void play();
  void stop();
  void animSettings();
  void displayNodeList();
  void displayInteraction();
  void displayAnimation();
  void displayNodeInterface();
  void reload();
  void copy();
  void paste();
  void selectAll();
  void group();
  void ungroup();
  void addNodeToList();
  void manageNodePaths();
  void saveNodeContent();
  void saveAnimNodeContent();
  void interactZoomIn();
  void interactZoomOut();
  void interactNoZoom();
  void interactViewAll();
  void graphZoomIn();
  void graphZoomOut();
  void graphNoZoom();
  void graphViewAll();

 private:
  void createActions();
  void createMenus();
  bool maybeSave();
  void clearAll();
  void initHelpWidget();
  void setCurrentFile(const QString &fileName);

  SceneSaver  _saver;
  SceneLoader _loader;
  QString     _currentFile;

  QMenu   *_fileMenu;
  QMenu   *_editMenu;
  QMenu   *_helpMenu;
  QMenu   *_animMenu;
  QMenu   *_windowMenu;
  QMenu   *_toolsMenu;
  QMenu   *_showHideMenu;
  QMenu   *_zoomInOutMenu;

  QAction *_newAct;
  QAction *_openAct;
  QAction *_saveAct;
  QAction *_saveAsAct;
  QAction *_exitAct;
  QAction *_copyAct;
  QAction *_pasteAct;
  QAction *_reloadAct;
  QAction *_selectAllAct;
  QAction *_helpAct;
  QAction *_aboutAct;
  QAction *_playAct;
  QAction *_stopAct;
  QAction *_nextFrameAct;
  QAction *_prevFrameAct;
  QAction *_firstFrameAct;
  QAction *_lastFrameAct;
  QAction *_settingsAct;
  QAction *_displayNodeListAct;
  QAction *_displayInteractionAct;
  QAction *_displayAnimationAct;
  QAction *_displayNodeInterfaceAct;
  QAction *_groupAct;
  QAction *_ungroupAct;
  QAction *_addCustomNodeToListAct;
  QAction *_manageNodePathsAct;
  QAction *_saveContentAct;
  QAction *_saveAnimContentAct;
  QAction *_interactZoomInAct;
  QAction *_interactZoomOutAct;
  QAction *_interactNoZoomAct;
  QAction *_interactViewAllAct;
  QAction *_graphZoomInAct;
  QAction *_graphZoomOutAct;
  QAction *_graphNoZoomAct;
  QAction *_graphViewAllAct;

  static QDir _currentPath;
  static const QString _defaultName;
};

#endif // MAIN_WINDOW_H
