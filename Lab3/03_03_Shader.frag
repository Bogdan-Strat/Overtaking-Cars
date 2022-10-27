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
		out_Color=vec4 (0.0, 0.0, 0.0, 0.0);
		break;
	case 2:
		out_Color=vec4 (1.0, 0.0, 1.0, 0.0);
		break;
	case 3:
		out_Color=vec4 (0.0, 0.0, 1.0, 1.0);
		break;
	case 4:
		out_Color=vec4 (0.55, 0.90, 1.0, 1.0);
		break;
	case 5:
		out_Color=vec4 (0.40, 0.40, 1.0, 1.0);
		break;
	case 6:
		out_Color=vec4 (0.80, 0.80, 1.0, 1.0);
		break;
	default:
		break;
  };
}
 