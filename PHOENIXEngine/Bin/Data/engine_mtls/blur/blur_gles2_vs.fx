attribute mediump vec3 modelPosition;
attribute mediump vec2 modelTCoord0;
varying mediump vec2 vertexTCoord0;
uniform mat4 PVWMatrix;
void main()
{
	gl_Position = PVWMatrix*vec4(modelPosition, 1.0);
	vertexTCoord0 = modelTCoord0;
}