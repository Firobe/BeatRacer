# BeatRacer
Projet GCC
Dépendances : OpenGL 3.3, OpenAL, freeALUT, GLFW 3 (GLM et stb_image déjà incluses)

Map (test.map) dans le format :
  RHO_1,THETA_1,PHI_1:NB_1
  ...
  RHO_N, THETA_N, PHI_N:NB_N
  où (RHO_(i+1), THETA_(i+1), PHI_(i+1)) indique la position en coordonnées sphériques du tronçon de route (i+1) par rapport au tronçon i. Les deux tronçons seront liés par une route bien orientée. 
  NB_i indique combien de fois répéter ce tronçon (utile pour les virages).

Joue par défaut test.wav
