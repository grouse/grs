
DECL_GL_PROC(void, glViewport, GLint x, GLint y, GLsizei width, GLsizei height);
DECL_GL_PROC(void, glAttachShader, GLuint program, GLuint shader);
DECL_GL_PROC(void, glDetachShader, GLuint program, GLuint shader);
DECL_GL_PROC(void, glBindAttribLocation, GLuint program, GLuint index, const GLchar *name);
DECL_GL_PROC(void, glCompileShader, GLuint shader);
DECL_GL_PROC(GLuint, glCreateProgram, void);
DECL_GL_PROC(GLuint, glCreateShader, GLenum type);
DECL_GL_PROC(GLuint, glDeleteShader, GLuint shader);
DECL_GL_PROC(GLuint, glDeleteProgram, GLuint program);
DECL_GL_PROC(void, glLinkProgram, GLuint program);
DECL_GL_PROC(void, glShaderSource, GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
DECL_GL_PROC(void, glGenBuffers, GLsizei n, GLuint *buffers);
DECL_GL_PROC(void, glGenVertexArrays, GLsizei n, GLuint *arrays);
DECL_GL_PROC(void, glBindVertexArray, GLuint array);
DECL_GL_PROC(void, glBindBuffer, GLenum target, GLuint buffer);
DECL_GL_PROC(void, glBindBufferBase, GLenum target, GLuint index, GLuint buffer);
DECL_GL_PROC(void, glBufferData, GLenum target, GLsizeiptr size, const void *data, GLenum usage); 
DECL_GL_PROC(void*, glMapBufferRange, GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
DECL_GL_PROC(void*, glMapNamedBufferRange, GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
DECL_GL_PROC(GLboolean, glUnmapBuffer, GLenum target);
DECL_GL_PROC(GLboolean, glUnmapNamedBuffer, GLuint buffer);
DECL_GL_PROC(void, glBufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const void * data);
DECL_GL_PROC(void, glVertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
DECL_GL_PROC(void, glEnableVertexAttribArray, GLuint index);
DECL_GL_PROC(void, glDisableVertexAttribArray, GLuint index);
DECL_GL_PROC(void, glUseProgram, GLuint program);
DECL_GL_PROC(void, glUniform1i, GLint location, GLint v0);
DECL_GL_PROC(void, glUniform1f, GLint location, GLfloat v0);
DECL_GL_PROC(void, glUniform2f, GLint location, GLfloat v0, GLfloat v1);
DECL_GL_PROC(void, glUniform3f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
DECL_GL_PROC(void, glUniform1fv, GLint location, GLsizei count, const GLfloat *value);
DECL_GL_PROC(void, glUniformMatrix3fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
DECL_GL_PROC(void, glUniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
DECL_GL_PROC(GLint, glGetUniformLocation, GLuint program, const GLchar *name);
DECL_GL_PROC(void, glGetShaderiv, GLuint shader, GLenum pname, GLint *params); 
DECL_GL_PROC(void, glGetShaderInfoLog, GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
DECL_GL_PROC(void, glGetProgramiv, GLuint program, GLenum pname, GLint *params);
DECL_GL_PROC(void, glGetProgramInfoLog, GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
DECL_GL_PROC(void, glGenTextures, GLsizei n, GLuint * textures);
DECL_GL_PROC(void, glDeleteTextures, GLsizei n, const GLuint * textures);
DECL_GL_PROC(void, glBindTexture, GLenum target, GLuint texture);
DECL_GL_PROC(void, glBlendFunc, GLenum sfactor, GLenum dfactor);
DECL_GL_PROC(void, glDebugMessageCallback, GLDEBUGPROC callback, void * userParam);
DECL_GL_PROC(void, glPolygonMode, GLenum face, GLenum mode);
DECL_GL_PROC(void, glBindBufferRange, GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
DECL_GL_PROC(void, glScissor, GLint x, GLint y, GLsizei width, GLsizei height);

DECL_GL_PROC(void, glPixelStorei, GLenum pname, GLint param);

DECL_GL_PROC(void, glTexParameteri, GLenum target, GLenum pname, GLint param);
DECL_GL_PROC(void, glTexStorage2D, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
DECL_GL_PROC(void, glTexImage2D, GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data);
DECL_GL_PROC(void, glTexSubImage2D, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels);
DECL_GL_PROC(void, glTextureSubImage2D, GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
DECL_GL_PROC(void, glMemoryBarrier, GLbitfield barriers);
