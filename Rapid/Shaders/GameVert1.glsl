attribute vec4 aPosition;
attribute vec3 aColor;
attribute vec3 aNormal;

varying vec3 vColor;

void main()
{
  gl_Position = gl_ModelViewProjectionMatrix * vec4(aPosition.x, aPosition.y, aPosition.z, aPosition.w);
  gl_FrontColor = vec4(aColor.r * aNormal.z, aColor.g * aNormal.z, aColor.b * aNormal.z, 1.0);
  vColor = vec3(0.4 * (1.0 - aNormal.y), 0.15 * (1.0 - aNormal.y), 0.1 * (1.0 - aNormal.y));
}
