#include "stdafx.h"
#include "Shader.h"

GLuint Shader::getID() {
	return shaderID;
}

Shader::Shader(const char * rutaVertex, const char * rutaFragment) {

	//Guardar en variables el texto en los codigos de shaders

	string codigoVertexShader;
	ifstream vertexShaderStream(rutaVertex, ios::in);
	if (vertexShaderStream.is_open())
	{
		string linea;
		while (getline(vertexShaderStream, linea)) {
			codigoVertexShader += linea + "\n";
		} 
		vertexShaderStream.close();
	}
	else {
		cout << "No se pudo abrir el archivo " << rutaVertex << endl;
	}

	string codigoFragmentShader;
	ifstream fragmentShaderStream(rutaFragment, ios::in);
	if (fragmentShaderStream.is_open()) {
		string linea;
		while (getline(fragmentShaderStream, linea)) {
			codigoFragmentShader += linea + "\n";
		}
		fragmentShaderStream.close();
	}
	else {
		cout << "No se pudo abrri el archivo" << rutaFragment << endl;
	}

	//Convertir de string a cadena de char
	const char* cadenaCodigoVertex = codigoVertexShader.c_str();
	const char* cadenaCodigoFragment = codigoFragmentShader.c_str();

	//1.- Creamos el programa de shaders
	shaderID = glCreateProgram();

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//2.- Cargar el codigo del shader
	glShaderSource(vertexShaderID, 1, &cadenaCodigoVertex, NULL);
	glShaderSource(fragmentShaderID, 1, &cadenaCodigoFragment, NULL);

	//3.- Compilar los cheiders
	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	//4.- Verificar errores de compilacion
	verificarCompilacion(vertexShaderID);
	verificarCompilacion(fragmentShaderID);

	//5.- Adjuntar shaders al programa
	glAttachShader(shaderID, vertexShaderID);
	glAttachShader(shaderID, fragmentShaderID);

	//6.- Vincular el programa
	glLinkProgram(shaderID);

	//7.- Verificar la vinculacion
	verificarVinculacion(shaderID);

	//8.- Usar programa
	glUseProgram(shaderID);
}

void Shader::verificarCompilacion(GLuint id) {
	GLint resultado = GL_FALSE;
	int longitudLog = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &resultado);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &longitudLog);

	if (longitudLog > 0) {
		vector<char> mensajeError(longitudLog);
		glGetShaderInfoLog(id, longitudLog, NULL, &mensajeError[0]);
		for (vector<char>::const_iterator i = mensajeError.begin();
			i != mensajeError.end();
			i++) {
			cout << *i;
		}
	}
}

void Shader::verificarVinculacion(GLuint id) {
	GLint estadoVinculacion, estadoValidacion;

	glGetProgramiv(id, GL_LINK_STATUS, &estadoVinculacion);
	if (estadoVinculacion == GL_FALSE)
	{
		cout << "No se pudo vincular programa" << endl;
	}

	glGetProgramiv(id, GL_VALIDATE_STATUS, &estadoValidacion);
	if (estadoValidacion == GL_FALSE)
	{
		cout << "No se pudo validar la vinculacion" << endl;
	}
}