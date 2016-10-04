#define POINT_CLAMP 	  	s0
#define BILINEAR_CLAMP    	s1
#define TRILINEAR_CLAMP   	s2
#define ANISOTROPIC_CLAMP 	s3
#define POINT_WRAP			s4
#define BILINEAR_WRAP		s5
#define TRILINEAR_WRAP		s6
#define ANISOTROPIC_WRAP	s7


float3x3 generateRotation_X_Matrix(float x)
{
  float3x3 fMatrix = 
  {
    1.0f,         0,       0,  // row 1
       0,    cos(x), -sin(x),  // row 2
       0,    sin(x),  cos(x)   // row 3             
  };

  return fMatrix;
}

float3x3 generateRotation_Y_Matrix(float y)
{
  float3x3 fMatrix = 
  {
     cos(y),    0, sin(y),  // row 1
          0, 1.0f,      0,  // row 2
    -sin(y),    0, cos(y)   // row 3             
  };

  return fMatrix;
}


float3x3 generateRotation_Z_Matrix(float z)
{
  float3x3 fMatrix = 
  {
    cos(z), -sin(z), 0,    // row 1
    sin(z),  cos(z), 0,    // row 2
    0,       0     , 1.0f // row 3              
  };

  return fMatrix;
}

float3x3 generateRotation_Rot_Matrix(float3 rot)
{
  return generateRotation_X_Matrix(rot.x) * generateRotation_Y_Matrix(rot.y) * generateRotation_Z_Matrix(rot.z);
}