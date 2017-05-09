#version 130

in vec3 outColor;
uniform int uColor;

void
main()
{
    if(outColor.r == 0.0 && outColor.g==1.0 && outColor.b==0.0 && uColor==1){
        gl_FragColor = vec4( 1, 0, 0, 1.0 );

    }else{
        gl_FragColor = vec4( outColor.x, outColor.y, outColor.z, 1.0 );
    }
}
