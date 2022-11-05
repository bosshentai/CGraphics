#include "Shader.h"
#include "./utils/Utils.h"

Shader::Shader()
{
  shaderID = 0;
  uniformModel = 0;
  uniformProjection = 0;
  uniformView = 0;
}

  void Shader::createFromStrings(const char *vertexCode, const char *fragmentCode)
  {
    compileShader(vertexCode, fragmentCode);
  }

  void Shader::compileShader(const char *vertexCode,const char *fragmentCode)
  {
    shaderID = glCreateProgram();

    if(!shaderID){
      std::cout << "Fail to create shader program!" << std::endl;
      return;
    }

  addShader(shaderID,vertexCode,GL_VERTEX_SHADER);
  addShader(shaderID,fragmentCode,GL_FRAGMENT_SHADER);

  compileProgram();

  }






void Shader::useShader(){
  glUseProgram(shaderID);
}


void Shader::clearShader(){
  if(shaderID != 0){
    glDeleteProgram(shaderID);
    shaderID = 0;
  }

  uniformModel = 0;
  uniformProjection = 0 ;
}


void Shader::addShader(GLuint program,const char* shaderCode,GLenum shaderType){
  GLuint theShader = glCreateShader(shaderType);


  const GLchar* theCode[1];
  theCode[0] = shaderCode;

  GLint codeLength[1];
  codeLength[0] = (GLint)strlen(shaderCode);


  glShaderSource(theShader,1,theCode,codeLength);
  glCompileShader(theShader);


  GLint result = 0 ;

  GLchar log[1024] = {0};


  std:: cout << "Compiling shader..." << std::endl;
  glGetShaderiv(theShader,GL_COMPILE_STATUS,&result);
  if(!result){
    glGetShaderInfoLog(theShader,sizeof(log),NULL,log);
    std::cout << "Failed to compile the %d shader! \n" <<log <<std::endl;
    printf("Fail to compile the %d shader: \n %s \n",shaderType,log);
    exit(1);
  }

  std::cout << "Complete." << std::endl;
  glAttachShader(program,theShader);

}


Shader::~Shader(){
  Shader::clearShader();
}

