// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef CODE_EDITOR
#define CODE_EDITOR

#include <QPlainTextEdit>
#include <QObject>
#include <QSyntaxHighlighter>

class LineNumberWidget;

class GLSLSyntaxHighLighter: public QSyntaxHighlighter {   
  
 public:    
  GLSLSyntaxHighLighter(QTextDocument* document);
  ~GLSLSyntaxHighLighter() {}
	
  void highlightBlock(const QString &text);

 private:
  void modifyBlockFormat(const QString &text,QRegExp &regexp,const QTextCharFormat &f);
  void modifyComments(const QString &text,QRegExp &regexp,const QTextCharFormat &f);

  QRegExp _builtinList;
  QRegExp _typeList;
  QRegExp _modifierList;
  QRegExp _preprocessList;
  QRegExp _blockSeparatorList;

  QTextCharFormat _builtinFormat;
  QTextCharFormat _typeFormat;
  QTextCharFormat _modifierFormat;
  QTextCharFormat _preprocessFormat;
  QTextCharFormat _blockSeparatorFormat;
  QTextCharFormat _commentFormat;

};


class CodeEditor : public QPlainTextEdit {
  Q_OBJECT
    
    public:
  CodeEditor(QWidget *parent=NULL,bool readOnly=false,unsigned int lineShift=0);

  void setLineShift(unsigned int lineShift);
  void lineNumberPaintEvent(QPaintEvent *event);
  int  lineNumberWidth();

  private slots:
  void updateLineNumberWidth();
  void updateLineNumber(const QRect &, int);
  void resizeNeeded();

 protected:
  void resizeEvent(QResizeEvent *event);

 private:
  GLSLSyntaxHighLighter *_glslHighlighter;
  LineNumberWidget      *_lineNumberWidget;
  unsigned int           _lineShift;
};


class LineNumberWidget : public QWidget {
 public:
  LineNumberWidget(CodeEditor *editor);
  QSize sizeHint() const;
  
 protected:
  void paintEvent(QPaintEvent *event);
  
 private:
  CodeEditor *_editor;
};


#endif // CODE_EDITOR
