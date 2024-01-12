uniform float scale;

varying vec2 texCoord;

void main(void)
{
  vec4 a = gl_Vertex;
  
  a.x = a.x * sin(scale);
  a.y = a.y * sin(scale);
  a.z = a.z * scale;
  
  texCoord = vec2(gl_MultiTexCoord0);
  gl_Position=gl_ModelViewProjectionMatrix *a;
  
}

