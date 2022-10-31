// Shader-ul de fragment / Fragment shader  
 #version 330

in vec4 ex_Color;
uniform int codCuloare;
 
out vec4 out_Color;

void main(void)
{
  switch (codCuloare)
  {
	case 0: 
	  out_Color = ex_Color;
	  break;
	case 1: 
		// car 1
		out_Color=vec4 (0.0f, 1.0f, 0.0f, 1.0f);
		break;
	case 2: 
		// car 2
		out_Color=vec4 (1.0f, 0.65f, 0.0f, 1.0f);
		break;
	case 3:
		// middle of the road
		out_Color = vec4 (1.0f, 1.0f, 1.0f, 1.0f);
	case 4:
		// windows
		out_Color = vec4 (0.24, 0.24, 0.24, 1.0);
		break;
	case 5:
		// headlights
		out_Color = vec4(0.98, 1.0, 0.65,  1.0);
		break;
	case 6:
		// rear lights
		out_Color = vec4(0.99, 0.1, 0.1, 1.0);
		break;
	case 7:
		// YOU WIN message
		out_Color = vec4(1.0, 1.0, 0.0, 1.0);
		break;
	default:
		break;
  };
}
 