
#include "Commons.h"
#include "CurveHermite.h"

static unsigned int programId;
#define  PI   3.14159265358979323846

mat4 Projection;
GLuint MatProj, MatModel;
int nv_P;
int nv_A;
int nPale = 2;

// viewport size
float width = 1280;
float height = 720;

float accelerazione = 0; //forza di accelerazione data dalla tastiera
float velocita = 1;
float accelerazione_verticale = -0.7;

//coordinate sul piano della posizione iniziale dell' aquilone
float posx = float(width) *0.3; 
float  posy = float(height) * 0.5;
float posSole = width * 0.1;
position* posNuvole = new position [5];

//Booleani per spostamenti
bool press_a = false;
bool press_d = false;
bool press_w = false;

//variabili per spostamenti di oggetti secondari
float angolo = 0.0;
float offset = float(height) * 0.10;
float* offsets = new float [nPale];

Figura  Montagna = {};
Figura Cielo= {};
Figura Tubo = {};
Figura Contorno_Tubo = {};
Figura* Pale_Eoliche = new Figura [8];
Figura Pala_Eolica= {};
Figura Sole = {};
Figura Aquilone = {};
Figura Coda_Aquilone = {};
extern Figura* Nuvola;


void init_offsets(float x, int n) {
	for (int i = 0; i <= n; i++) {
		offsets[i] = width * x * i;
	}
	for (int i = 0; i < 5; i++) {
		posNuvole[i].x = width * (rand() % 100 + 1) / 100;
		posNuvole[i].y = height * (rand() % 100 + 1) / 100;
	}
}

void crea_VAO_Vector(Figura* fig)
{

	glGenVertexArrays(1, &fig->VAO);
	glBindVertexArray(fig->VAO);
	//Genero , rendo attivo, riempio il VBO della geometria dei vertici
	glGenBuffers(1, &fig->VBO_G);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_G);
	glBufferData(GL_ARRAY_BUFFER, fig->vertici.size() * sizeof(vec3),fig->vertici.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//Genero , rendo attivo, riempio il VBO dei colori
	glGenBuffers(1, &fig->VBO_C);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_C);
	glBufferData(GL_ARRAY_BUFFER, fig->colors.size() * sizeof(vec4), fig->colors.data(), GL_STATIC_DRAW);
	//Adesso carico il VBO dei colori nel layer 2
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

}

void costruisci_aquilone(vec4 color_top, vec4 color_bot, Figura* Aquilone)
{
	int i;
	float stepA = (2 * PI) / Aquilone->nTriangles;
	float t;
	

	Aquilone->vertici.push_back(vec3(0.0, 0.0, 0.0));
	Aquilone->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));

	for (i = 0; i <= Aquilone->nTriangles; i ++)
	{	t = (float)i * stepA;

		if (i % 2 == 0) {
			Aquilone->vertici.push_back(vec3(cos(t), sin(t), 0.0));
			Aquilone->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));
			
			t = (float)(i + 1) * stepA;

			Aquilone->vertici.push_back(vec3(cos(t), sin(t), 0.0));
			Aquilone->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));

			Aquilone->vertici.push_back(vec3(0, 0, 0.0));
			Aquilone->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));

		}
		else {
			Aquilone->vertici.push_back(vec3(cos(t), sin(t), 0.0));
			Aquilone->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));

			t = (float)(i + 1) * stepA;

			Aquilone->vertici.push_back(vec3(cos(t), sin(t), 0.0));
			Aquilone->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));

			Aquilone->vertici.push_back(vec3(0, 0, 0.0));
			Aquilone->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));

		}
		
	}
	nv_A = Aquilone->vertici.size();

}

void costruisci_coda_aquilone (vec4 color_top, vec4 color_bot, Figura* Coda_Aquilone) {

	int i = 0;

	float step = 1 / (float)120;
	float frequenza = PI * 6;
	float t;
	int n_vertici = 0;
	
	for (i = 0; i < 120; i += 1)
	{
		t = i / (float)120 * frequenza;
		Coda_Aquilone->vertici.push_back(vec3(i * step, 0, 0));
		Coda_Aquilone->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));
		Coda_Aquilone->vertici.push_back(vec3(i*step, abs(sin(t)), 0.0));
		Coda_Aquilone->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	}
	Coda_Aquilone->nv = Coda_Aquilone->vertici.size();


}

void costruisci_pala_eolica(vec4 color_top, vec4 color_bot, Figura* Pala_Eolica)
{
int i;
float stepA = (2 * PI) /( Pala_Eolica->nTriangles);
float t;

 
Pala_Eolica->vertici.push_back(vec3(0.0,0.0, 0.0));
Pala_Eolica->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));

for (i = 0; i <= Pala_Eolica->nTriangles; i+=2)
{
	t = (float)i * stepA;
	
	Pala_Eolica->vertici.push_back(vec3(cos(t)*2, sin(t)*2, 0.0));
	Pala_Eolica->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));

	t = (float)(i+1) * stepA;

	Pala_Eolica->vertici.push_back(vec3(cos(t)*2, sin(t)*2, 0.0));
	Pala_Eolica->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));
	
	Pala_Eolica->vertici.push_back(vec3(0.0, 0.0, 0.0));
	Pala_Eolica->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
}
nv_P = Pala_Eolica->vertici.size();

//Sostegno Pala Eolica
Pala_Eolica->vertici.push_back(vec3(-0.1, -1.0, 0.0));
Pala_Eolica->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
Pala_Eolica->vertici.push_back(vec3(0.9, -1.0, 0.0));
Pala_Eolica->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
Pala_Eolica->vertici.push_back(vec3(-0.1, 0.0, 0.0));
Pala_Eolica->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
Pala_Eolica->vertici.push_back(vec3(0.9, 0.0, 0.0));
Pala_Eolica->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));
Pala_Eolica->nv = Pala_Eolica->vertici.size();

}

void costruisci_Sole(vec4 color_top, vec4 color_bot, vec4 color_top_alone, vec4 color_bot_alone, Figura* Sole) {
	
	int i;
	float stepA = (2 * PI) / Sole->nTriangles;
	float t;


	Sole->vertici.push_back(vec3(0.0, 0.0, 0.0));
	Sole->colors.push_back(vec4(color_bot.r,color_bot.g,color_bot.b,color_bot.a));

	for (i = 0; i <= Sole->nTriangles; i++)
	{
		t = (float)i * stepA;
		
		Sole->vertici.push_back(vec3(2*cos(t), 2*sin(t),0.0));
		//Colore 
		Sole->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
 	}

	//Costruzione Alone
	Sole->vertici.push_back(vec3(0.0, 0.0, 0.0));
	Sole->colors.push_back(vec4(color_bot_alone.r, color_bot_alone.g, color_bot_alone.b, color_bot_alone.a));
	for (i = 0; i <= Sole->nTriangles; i++)
	{
		t = (float)i * stepA;

		Sole->vertici.push_back(vec3(3.0*cos(t), 3.0*sin(t), 0.0));
		//Colore 
		 
		Sole->colors.push_back(vec4(color_top_alone.r, color_top_alone.g, color_top_alone.b, color_top_alone.a));

	}
	Sole->nv = Sole->vertici.size();
	
}

void costruisci_cielo(vec4 color_top, vec4 color_bot, Figura* cielo)
{


	cielo->vertici.push_back(vec3(0.0, 0.0,0.0));
	cielo->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_top.a));
	cielo->vertici.push_back(vec3(1.0, 0.0, 0.0));
	cielo->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));
	cielo->vertici.push_back(vec3(0.0, 1.0, 0.0));
	cielo->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	cielo->vertici.push_back(vec3(1.0, 1.0, 0.0));
	cielo->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));

	cielo->nv = cielo->vertici.size();
	 
	//Costruzione matrice di Modellazione Sole, che rimane la stessa(non si aggiorna)
	cielo->Model = mat4(1.0);
	cielo->Model = translate(cielo->Model, vec3(0.0, 0.0, 0.0));
	cielo->Model = scale(cielo->Model, vec3(float(width), float(height), 1.0));
}

void costruisci_tubo(vec4 color_top, vec4 color_bot, Figura* tubo)
{


	tubo->vertici.push_back(vec3(0.0, 0.0, 0.0));
	tubo->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	tubo->vertici.push_back(vec3(0.0, 1.0, 0.0));
	tubo->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	tubo->vertici.push_back(vec3(1.0, 0.0, 0.0));
	tubo->colors.push_back(vec4(color_top.r, color_top.g, color_top.b, color_top.a));
	tubo->vertici.push_back(vec3(1.0, 1.0, 0.0));
	tubo->colors.push_back(vec4(color_bot.r, color_bot.g, color_bot.b, color_bot.a));

	tubo->nv = tubo->vertici.size();
}

double lerp(double a, double b, double amount) {

	//Interpolazione lineare tra a e b secondo amount

	return (1 - amount) * a + amount * b;
}

void INIT_SHADER(void)
{
	GLenum ErrorCheckValue = glGetError();

	char* vertexShader = (char*)"ProgettoCG/vertexShader_M.glsl";
	char* fragmentShader = (char*)"ProgettoCG/fragmentShader_M.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	glUseProgram(programId);
}

void INIT_VAO(void)
{
	vec4 col_top = { 0.6471, 0.3020,1.0,1.0 };
	vec4 col_bottom = { 0.0, 0.4980,1.0,1.0 };
	costruisci_cielo(col_top, col_bottom, &Cielo);
	crea_VAO_Vector(&Cielo);


	Aquilone.nTriangles = 4;
	col_top = { 1.0,0.0,0.0,1.0 };
	col_bottom = { 1.0, 0.8, 0.0, 1.0 };
	costruisci_aquilone(col_top, col_bottom, &Aquilone);
	crea_VAO_Vector(&Aquilone);


	col_top = { 1.0,0.0,0.0,1.0 };
	col_bottom = { 1.0, 0.8, 0.0, 1.0 };
	costruisci_coda_aquilone(col_top, col_bottom, &Coda_Aquilone);
	crea_VAO_Vector(&Coda_Aquilone);


	col_top = vec4{ 0.1333, 0.5451, 0.1333, 1.0000 };
	col_bottom = vec4{ 0.6784, 1.0,0.1843, 1.0000 };
	costruisci_tubo(col_top, col_bottom, &Tubo);
	crea_VAO_Vector(&Tubo);


	col_top = { 1.0,1.0,7.0,1.0 };
	col_bottom = { 0.5, 0.9, 0.5, 0.2 };
	for (int i = 0; i < nPale; i++) {
		Pale_Eoliche[i].nTriangles = 8;
		costruisci_pala_eolica(col_top, col_bottom, &Pale_Eoliche[i]);
		crea_VAO_Vector(&Pale_Eoliche[i]);
	}

	col_top = { 1.0,1.0,7.0,1.0 };
	col_bottom = { 0.5, 0.9, 0.5, 0.2 };
	vec3 point = { 0.155, 0.263, 0.0 };
	for (int i = 0; i < tot_nuvole; i++) {
		costruisci_forma(col_top, col_bottom, &Nuvola[i], "ProgettoCG/nuvola.json", point);
		crea_VAO_Vector(&Nuvola[i]);
	}

	 Sole.nTriangles = 30;
	 col_top = vec4{ 1.0, 0.9, 0.9, 0.8 };
	 col_bottom = vec4{ 1.0,0.8627,0.0, 1.0000 };
	 vec4 col_top_alone = { 1.0, 1.0, 1.0, 0.0 };
	 vec4 col_bottom_alone = { 1.0,0.8627,0.0, 1.0000 };
	 costruisci_Sole(col_top, col_bottom, col_top_alone, col_bottom_alone, &Sole);
	 crea_VAO_Vector(&Sole);
	 
	  //Costruzione della matrice di Proiezione
	  Projection = ortho(0.0f, float(width), 0.0f, float(height));
	  MatProj = glGetUniformLocation(programId, "Projection");
	  MatModel = glGetUniformLocation(programId, "Model");

}

void drawScene(void)
{
	 
	glClearColor(0.0,0.0,0.0, 0.5);
	glClear(GL_COLOR_BUFFER_BIT);
	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
 

	//Disegno Cielo
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Cielo.Model));
	glBindVertexArray(Cielo.VAO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, Cielo.nv);
	glBindVertexArray(0);

	
	//Disegno Sole
	glBindVertexArray(Sole.VAO);
	//Costruzione matrice di Moellazione Sole, che rimane la stessa(non si aggiorna)
	Sole.Model = mat4(1.0);
	Sole.Model = translate(Sole.Model, vec3(posSole, float(height) * 0.8, 0.0));
	Sole.Model = scale(Sole.Model, vec3(30.0, 30.0, 1.0));
	 glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Sole.Model));

	//Disegno raggi sole
	glDrawArrays(GL_TRIANGLE_FAN, (Sole.nTriangles) + 2, (Sole.nTriangles) + 2);
	//Disegn0 tondo del sole	  
	glDrawArrays(GL_TRIANGLE_FAN, 0, (Sole.nTriangles) + 2);
    glBindVertexArray(0);


	//Disegno pale eoliche
	 for (int i = 0; i < nPale; i++) {

		 glBindVertexArray(Pale_Eoliche[i].VAO);
		 //Disegno il sostegno
		 //Definisco la matrice di Trasformazione per il sostegno
		 Pale_Eoliche[i].Model = mat4(1.0);
		 Pale_Eoliche[i].Model = translate(Pale_Eoliche[i].Model, vec3(offsets[i], i%2 == 0? float(height) * 0.15 : float(height) * 0.3, 0.0));
		 Pale_Eoliche[i].Model = scale(Pale_Eoliche[i].Model, vec3(3.0, 500, 1.0));
		 //Model = rotate(Model, radians(angolo), vec3(0.0f, 0.0f, 1.0f));
		 glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Pale_Eoliche[i].Model));


		 glDrawArrays(GL_TRIANGLE_STRIP, Pale_Eoliche[i].nv - 4, 4);

		 //Disegna la Pala
		  //Definisco la matrice di trasformazione per la Pala
		 Pale_Eoliche[i].Model = mat4(1.0);
		 Pale_Eoliche[i].Model = translate(Pale_Eoliche[i].Model, vec3(offsets[i], i % 2 == 0 ? float(height) * 0.15 : float(height) * 0.3, 0.0)); //dove deve iniziare la matrice: vec3(x, y, z(?) )
		 Pale_Eoliche[i].Model = scale(Pale_Eoliche[i].Model, vec3(60, 60, 1.0)); //vec3(larghezza, altezza,..quanto si chiude(?) )
		 Pale_Eoliche[i].Model = rotate(Pale_Eoliche[i].Model, radians(angolo), vec3(0.0f, 0.0f, 1.0f));//vec3(gira orizzontalmente, .. verticalmente, ruota)
		 glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Pale_Eoliche[i].Model));

		 glDrawArrays(GL_TRIANGLES, 0, nv_P);
	 }


	 //Disegno nuvola
	 for (int i = 0; i < tot_nuvole; i++) {
		 //Definisco la matrice di Modellazione della nuvola
		 Nuvola[i].Model = mat4(1.0);
		 Nuvola[i].Model = translate(Nuvola[i].Model, vec3(posNuvole[i].x, posNuvole[i].y, 0.0));
		 Nuvola[i].Model = scale(Nuvola[i].Model, vec3(150.0, 75.0, 1.0));
		 glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Nuvola[i].Model));
		 glBindVertexArray(Nuvola[i].VAO);
		 glDrawArrays(GL_TRIANGLE_FAN, 0, Nuvola[i].nv);
		 glBindVertexArray(0);
	 }


	//Disegna tubo inferiore
	 Tubo.Model = mat4(1.0);
	 Tubo.Model = scale(Tubo.Model, vec3(float(width), float(height) * 0.1, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Tubo.Model));
	glBindVertexArray(Tubo.VAO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, Tubo.nv);
	glBindVertexArray(0);

	//Disegna tubo superiore
	Tubo.Model = mat4(1.0);
	Tubo.Model = translate(Tubo.Model, vec3(0, float(height)*0.9, 0.0)); 
	Tubo.Model = scale(Tubo.Model, vec3(float(width), float(height) * 0.1, 1.0));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Tubo.Model));
	glBindVertexArray(Tubo.VAO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, Tubo.nv);
	glBindVertexArray(0);


	//Disegno aquilone
	float delta = 0;
	glBindVertexArray(Aquilone.VAO);
	Aquilone.Model = mat4(1.0);
	Aquilone.Model = translate(Aquilone.Model, vec3(posx, posy, 0.0)); 
	Aquilone.Model = scale(Aquilone.Model, vec3(80.0, 40.0, 1.0)); 
	Aquilone.Model = rotate(Aquilone.Model, radians(float (0)), vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Aquilone.Model));
	glDrawArrays(GL_TRIANGLES, 0, nv_A);
	glBindVertexArray(0);

	//Disegno coda aquilone
	delta = 10;
	Coda_Aquilone.Model = mat4(1.0);
	Coda_Aquilone.Model = translate(Coda_Aquilone.Model, vec3(posx - 180, posy, 0.0));
	Coda_Aquilone.Model = scale(Coda_Aquilone.Model, vec3(100, 7, 1.0));
	Coda_Aquilone.Model = rotate(Coda_Aquilone.Model, radians(delta), vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Coda_Aquilone.Model));
	glBindVertexArray(Coda_Aquilone.VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, Coda_Aquilone.nv);
	glBindVertexArray(0);

	delta = 190;
	Coda_Aquilone.Model = mat4(1.0);
	Coda_Aquilone.Model = translate(Coda_Aquilone.Model, vec3(posx - 80, posy, 0.0));
	Coda_Aquilone.Model = scale(Coda_Aquilone.Model, vec3(100, 7, 1.0));
	Coda_Aquilone.Model = rotate(Coda_Aquilone.Model, radians(-delta), vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Coda_Aquilone.Model));
	glBindVertexArray(Coda_Aquilone.VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, Coda_Aquilone.nv);
	glBindVertexArray(0);

	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	srand(time(NULL));

	init_offsets(0.5, nPale);

	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Giochino aquilone");
	glutDisplayFunc(drawScene);

	glutKeyboardFunc(keyboardPressedEvent);
	glutKeyboardUpFunc(keyboardReleasedEvent);

	//gestione animazione
	glutTimerFunc(66, update, 0);

	glewExperimental = GL_TRUE;
	glewInit();
	INIT_SHADER();
	INIT_VAO();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glutMainLoop();
}


