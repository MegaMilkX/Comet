#ifdef COMPILING_VS

in vec3 vertexPosition_modelspace;
in vec3 vertexColor;
in vec3 vertexNormal;
in vec3 uvw;

uniform mat4 _m;
uniform mat4 _v;
uniform mat4 _p;

void main()
{
	gl_Position = _p * _v * _m * vertexPosition_modelspace;
}

#elif COMPILING_PS

#endif