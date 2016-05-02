# BeatRacer
Projet GCC
## Dépendances
OpenGL 3.3, OpenAL, GLFW 3 (GLM et stb_image déjà incluses)

## Map
  RHO\_1,THETA\_1,PHI\_1:NB\_1  
  ...  
  RHO~N~, THETA~N~, PHI~N~:NB~N~  
  où (RHO\_i+1, THETA\_i+1, PHI\_i+1) indique la position en coordonnées sphériques du tronçon de route (i+1) par rapport au tronçon i. Les deux tronçons seront liés par une route bien orientée.   
  NB\_i indique combien de fois répéter ce tronçon (utile pour les virages).

Joue par défaut test.wav
