#include "note.h"

using namespace std;

Note::Note(){
  loadModel("note");
}

Note::Note(float pos, int col) : Model(){
  loadModel("note");
  _column = col;
  _position = pos;
}

Note::~Note(){
}

NoteHandler::NoteHandler(string path, Map& map){
  float pos;
  int col;
  string buffer;
  ifstream input;
  path = "res/map/" + path + ".nt";
  input.open(path.c_str());
  if(!input.is_open()){
    cout << "!! Can't open " << path << " : halting !!" << endl;
    exit(EXIT_FAILURE);
  }
  while(getline(input, buffer)){
    sscanf(buffer.c_str(), "%f,%d", &pos, &col);
    for(int i = 0; i < 4; i++){
      if(col % 2){
	notes.push_back(Note(pos,i));
	notes.back().translate(map.getWorldCoordinates(glm::vec3(pos,ROAD_WIDTH*(1+i)/4+0.5,0)));
	glm::vec2 angles= map.getWorldAngles(pos);
	notes.back().rotate(angles[1], glm::vec3(0.,0.,1.));
	notes.back().rotate(angles[2], glm::vec3(0.,1.,0.));
      }
      col /= 2;
    }
  }  
}

NoteHandler::~NoteHandler(){
}

void NoteHandler::draw(Video& vid){
  for(int i = 0; i < notes.size() ; i++)
    notes[i].draw(vid);
}

