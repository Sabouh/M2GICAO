// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef CUSTOM_NODE_H
#define CUSTOM_NODE_H

#include "core/nodeInterface.h"

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QSpinBox>

class GraphWidget;

class CustomNodeWidget : public QDialog  {
  Q_OBJECT

 public:
  CustomNodeWidget(NodeInterface *node,GraphWidget *graphWidget);
    
  public slots:
  void confirmClicked();
  void cancelClicked();
 
  /* private slots: */
  /* void inputTextChanged(const QString &text); */
  /* void outputTextChanged(const QString &text); */

 private:
  NodeInterface *_node;
  GraphWidget   *_graphWidget;

  QLabel *_title;
  QLabel *_stringIdLabel;
  QLabel *_versionLabel;
  QLabel *_nameLabel;
  QLabel *_pathLabel;
  QLabel *_descLabel;
  QLabel *_helpLabel;
  //QLabel *_inputsLabel;
  //QLabel *_outputsLabel;
  QLabel *_directoryLabel;

  QLineEdit *_stringIdEdit;
  QSpinBox  *_versionEdit;
  QLineEdit *_nameEdit;
  QLineEdit *_pathEdit;
  QLineEdit *_descEdit;
  QTextEdit *_helpEdit;
  //QComboBox *_inputsEdit;
  //QComboBox *_outputsEdit;
  QComboBox *_directory;

  QDialogButtonBox *_buttons;
};

class ManagePathsWidget : public QDialog  {
  Q_OBJECT

 public:
  ManagePathsWidget();
    
  public slots:
  void confirmClicked();
  void cancelClicked();
 
  private slots:
  void addClicked();
  void editClicked();
  void delClicked();

 private:
  QLabel      *_title;
  QLabel      *_ruleLabel;
  QListWidget *_pathsList;
  QPushButton *_addButton;
  QPushButton *_editButton;
  QPushButton *_delButton;

  QDialogButtonBox *_buttons;
};

#endif // CUSTOM_NODE_H
