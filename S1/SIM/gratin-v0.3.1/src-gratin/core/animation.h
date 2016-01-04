// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef ANIMATION_H
#define ANIMATION_H

#include "core/sceneIOTags.h"

class Animation : public IOData {
 public:
 Animation(unsigned int firstFrame=0,unsigned int lastFrame=99,unsigned int fps=24) 
    : _firstFrame(firstFrame),
    _lastFrame(lastFrame),
    _currentFrame(_firstFrame),
    _fps(fps) {
  }
    
  virtual ~Animation() {}

  // assume that firstFrame<=lastFrame and _currentFrame in [_firstFrame,_lastFrame]

  inline unsigned int firstFrame  () const {return _firstFrame;  }
  inline unsigned int lastFrame   () const {return _lastFrame;    }
  inline unsigned int currentFrame() const {return _currentFrame;}
  inline unsigned int nbFrames    () const {return _lastFrame-_firstFrame+1;}
  inline unsigned int fps         () const {return _fps;}

  inline void setFirstFrame  (unsigned int firstFrame  ) {_firstFrame   = firstFrame;  }
  inline void setLastFrame   (unsigned int lastFrame   ) {_lastFrame     = lastFrame;  }
  inline void setCurrentFrame(unsigned int currentFrame) {_currentFrame = currentFrame;}
  inline void setFps         (unsigned int fps         ) {_fps = fps;                  }

  inline void incrementCurrentFrame() {_currentFrame = _currentFrame==_lastFrame ? _firstFrame : _currentFrame+1;}
  inline void decrementCurrentFrame() {_currentFrame = _currentFrame==_firstFrame ? _lastFrame : _currentFrame-1;}

  inline void goToFirst() {_currentFrame = _firstFrame;}
  inline void goToLast () {_currentFrame = _lastFrame; }

  // load and save 
  inline bool save(SceneSaver  *saver );
  inline bool load(SceneLoader *loader);

 private:
  unsigned int _firstFrame;
  unsigned int _lastFrame;
  unsigned int _currentFrame;
  unsigned int _fps;
};

inline bool Animation::save(SceneSaver *saver) {
  bool ret = true;
  if(!saver->saveUint("firstFrame"  ,_firstFrame))   ret = false;
  if(!saver->saveUint("lastFrame"   ,_lastFrame))    ret = false;
  if(!saver->saveUint("currentFrame",_currentFrame)) ret = false;
  if(!saver->saveUint("fps"         ,_fps))          ret = false;
  return ret;
}

inline bool Animation::load(SceneLoader *loader) {
  bool ret = true;
  if(!loader->loadUint("firstFrame"  ,_firstFrame))   ret = false;
  if(!loader->loadUint("lastFrame"   ,_lastFrame))    ret = false;
  if(!loader->loadUint("currentFrame",_currentFrame)) ret = false;
  if(!loader->loadUint("fps"         ,_fps))          ret = false;
  return ret;
}



#endif // ANIMATION_H
