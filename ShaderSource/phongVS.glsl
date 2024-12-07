#version 460 core


//#version 460 core
//
//uniform mat4 projectionTrans;
//uniform mat4 viewTrans;
//uniform mat4 worldTrans;
//
//struct Vertex
//{    
//    vec3 position;
//	vec2 tex;
//    vec3 normal;
//};
//
//struct VertexOut
//{    
//    vec4 position;
//	vec2 tex;
//    vec3 normal;
//};
//
//layout(location = 0) in vec3 inPosition;
//layout(location = 1) in vec2 inTex;
//layout(location = 2) in vec3 inNormal;
//
//out VertexOut outData;
//
//void main()
//{
//	Vertex vertex;
//	vertex.position = inPosition;
//	vertex.tex = inTex;
//	vertex.normal = inNormal;
//
//	outData.position = worldTrans * vec4(vertex.position, 1.0);
//	outData.tex = vertex.tex;
//	outData.normal = vec3(transpose(inverse(worldTrans)) * vec4(vertex.normal, 0.0));
//	gl_Position = projectionTrans * viewTrans * outData.position;
//}