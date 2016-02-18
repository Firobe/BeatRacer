#ifndef NOTE_H
#define NOTE_H

#include "model.h"
#include "map.h"

class Note : public Model {
 public:
  Note();
  Note(float, int);
  ~Note();
 private:
  int _column;
  float _position;
};

class NoteHandler {
 public:
  NoteHandler(std::string, Map&);
  ~NoteHandler();
  void draw(Video&);
 private:
  std::vector<Note>notes;
  
};


#endif //NOTE_H
