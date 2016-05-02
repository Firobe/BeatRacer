# BeatRacer
Projet GCC
## Dépendances
OpenGL 3.3, OpenAL, GLFW 3 (GLM et stb_image déjà incluses)

## Map
  RHO~1~,THETA~1~,PHI~1~:NB~1~  
  ...  
  RHO~N~, THETA~N~, PHI~N~:NB~N~  
  où (RHO~i+1~, THETA~i+1~, PHI~i+1~) indique la position en coordonnées sphériques du tronçon de route (i+1) par rapport au tronçon i. Les deux tronçons seront liés par une route bien orientée.   
  NB~i~ indique combien de fois répéter ce tronçon (utile pour les virages).

Joue par défaut test.wav
