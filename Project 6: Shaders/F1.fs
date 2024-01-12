uniform sampler2D myTexture;
varying vec2 texCoord;
uniform int colorMode; // Add a uniform to control color manipulation

void main() {
    vec4 texColor = texture2D(myTexture, texCoord);

    if (colorMode == 1) {
        // If red component is dominant, turn it blue
        if (texColor.r > texColor.g && texColor.r > texColor.b) {
            texColor.b = texColor.r;
            texColor.r = 0.0;
        }
    } else if (colorMode == 2) {
        // If green component is dominant, turn it blue
        if (texColor.g > texColor.r && texColor.g > texColor.b) {
            texColor.b = texColor.g;
            texColor.g = 0.0;
        }
    }
    
    gl_FragColor = texColor;
}
