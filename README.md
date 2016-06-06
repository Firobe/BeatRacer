# BeatRacer
Projet GCC
## Dépendances
OpenGL 3.3+, OpenAL, GLFW 3

## Map
  RHO\_1,THETA\_1,PHI\_1:NB\_1  
  ...  
  RHO\_N, THETA\_N, PHI\_N:NB\_N  
  où (RHO\_(i+1), THETA\_(i+1), PHI\_(i+1)) indique la position en coordonnées sphériques du tronçon de route (i+1) par rapport au tronçon i. Les deux tronçons seront liés par une route bien orientée.   
  NB\_i indique combien de fois répéter ce tronçon (utile pour les virages).

## Contrôles
Haut/Bas : changer de palier de vitesse (vaisseau et musique)  
Droite/Gauche : rotation du vaisseau  
Espace + Haut : initie le boost (use la barre)  
Espace + Bas : arrête le boost  
Espace + Gauche/Droite : rotation du vaisseau plus violente  
WXCV :touches notes  
Alt : passage en caméra libre (contrôlable avec ZQDS)
Ctrl gauche : libérer le curseur  
Ctrl droit : lier le curseur  
Echap : quitter

Joue par défaut test.ogg
