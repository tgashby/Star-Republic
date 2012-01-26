all:
	g++ -o game MainFull.cpp CMeshLoader.cpp CShader.cpp CMesh.cpp Enemy.cpp GameObject.cpp InputManager.cpp Player.cpp Camera.cpp Collision.cpp Bullet.cpp Bullets.cpp Map.cpp HUD.cpp Turret.cpp Turrets.cpp MeshParser.cpp -DGL_GLEXT_PROTOTYPES -lglut -lGL -lGLU

mac:
	g++ -o game MainFull.cpp CMeshLoader.cpp CShader.cpp CMesh.cpp Enemy.cpp GameObject.cpp InputManager.cpp Player.cpp Camera.cpp Collision.cpp Bullet.cpp Bullets.cpp Map.cpp HUD.cpp Turret.cpp Turrets.cpp MeshParser.cpp -DGL_GLEXT_PROTOTYPES -framework GLUT -framework OpenGL -framework Cocoa

run: 
	./game
