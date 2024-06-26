/*
Niniejszy program jest wolnym oprogramowaniem; mo�esz go
rozprowadza� dalej i / lub modyfikowa� na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundacj� Wolnego
Oprogramowania - wed�ug wersji 2 tej Licencji lub(wed�ug twojego
wyboru) kt�rej� z p�niejszych wersji.

Niniejszy program rozpowszechniany jest z nadziej�, i� b�dzie on
u�yteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domy�lnej
gwarancji PRZYDATNO�CI HANDLOWEJ albo PRZYDATNO�CI DO OKRE�LONYCH
ZASTOSOWA�.W celu uzyskania bli�szych informacji si�gnij do
Powszechnej Licencji Publicznej GNU.

Z pewno�ci� wraz z niniejszym programem otrzyma�e� te� egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
je�li nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#include "shaderprogram.h"



//Procedura wczytuje plik do tablicy znak�w.
char* ShaderProgram::readFile(const char* fileName) {
	int filesize;
	FILE* plik;
	char* result;

#pragma warning(suppress : 4996) //Wy��czenie b��du w Visual Studio wynikaj�ce z nietrzymania si� standard�w przez Microsoft.
	plik = fopen(fileName, "rb");
	if (plik != NULL) {
		fseek(plik, 0, SEEK_END);
		filesize = ftell(plik);
		fseek(plik, 0, SEEK_SET);
		result = new char[filesize + 1];
#pragma warning(suppress : 6386) //Wy��czenie b��du w Visual Studio wynikaj�cego z b��dnej analizy statycznej kodu.
		int readsize = fread(result, 1, filesize, plik);
		result[filesize] = 0;
		fclose(plik);

		return result;
	}

	return NULL;

}

//Metoda wczytuje i kompiluje shader, a nast�pnie zwraca jego uchwyt
GLuint ShaderProgram::loadShader(GLenum shaderType, const char* fileName) {
	//Wygeneruj uchwyt na shader
	GLuint shader = glCreateShader(shaderType);//shaderType to GL_VERTEX_SHADER, GL_GEOMETRY_SHADER lub GL_FRAGMENT_SHADER
	//Wczytaj plik ze �r�d�em shadera do tablicy znak�w
	const GLchar* shaderSource = readFile(fileName);
	//Powi�� �r�d�o z uchwytem shadera
	glShaderSource(shader, 1, &shaderSource, NULL);
	//Skompiluj �r�d�o
	glCompileShader(shader);
	//Usu� �r�d�o shadera z pami�ci (nie b�dzie ju� potrzebne)
	delete[]shaderSource;

	//Pobierz log b��d�w kompilacji i wy�wietl
	int infologLength = 0;
	int charsWritten = 0;
	char* infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1) {
		infoLog = new char[infologLength];
		glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		delete[]infoLog;
	}

	//Zwr�� uchwyt wygenerowanego shadera
	return shader;
}

ShaderProgram::ShaderProgram(const char* vertexShaderFile, const char* geometryShaderFile, const char* fragmentShaderFile) {
	//Wczytaj vertex shader
	printf("Loading vertex shader...\n");
	vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderFile);

	//Wczytaj geometry shader
	if (geometryShaderFile != NULL) {
		printf("Loading geometry shader...\n");
		geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryShaderFile);
	}
	else {
		geometryShader = 0;
	}

	//Wczytaj fragment shader
	printf("Loading fragment shader...\n");
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderFile);

	//Wygeneruj uchwyt programu cieniuj�cego
	shaderProgram = glCreateProgram();

	//Pod��cz do niego shadery i zlinkuj program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	if (geometryShaderFile != NULL) glAttachShader(shaderProgram, geometryShader);
	glLinkProgram(shaderProgram);

	//Pobierz log b��d�w linkowania i wy�wietl
	int infologLength = 0;
	int charsWritten = 0;
	char* infoLog;

	glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 1)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(shaderProgram, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		delete[]infoLog;
	}

	printf("Shader program created \n");
}

ShaderProgram::~ShaderProgram() {
	//Od��cz shadery od programu
	glDetachShader(shaderProgram, vertexShader);
	if (geometryShader != 0) glDetachShader(shaderProgram, geometryShader);
	glDetachShader(shaderProgram, fragmentShader);

	//Wykasuj shadery
	glDeleteShader(vertexShader);
	if (geometryShader != 0) glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);

	//Wykasuj program
	glDeleteProgram(shaderProgram);
}


//W��cz u�ywanie programu cieniuj�cego reprezentowanego przez aktualny obiekt
void ShaderProgram::use() {
	glUseProgram(shaderProgram);
}

//Pobierz numer slotu odpowiadaj�cego zmiennej jednorodnej o nazwie variableName
GLuint ShaderProgram::u(const char* variableName) {
	return glGetUniformLocation(shaderProgram, variableName);
}

//Pobierz numer slotu odpowiadaj�cego atrybutowi o nazwie variableName
GLuint ShaderProgram::a(const char* variableName) {
	return glGetAttribLocation(shaderProgram, variableName);
}