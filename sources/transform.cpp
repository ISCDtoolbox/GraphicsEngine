#include <glic/transform.h>

void CglicTransform::setTranslation(glm::vec3 translation){
  tr += translation;
}

void CglicTransform::setRotation(glm::mat4 M){
  rot = M;
}
/*
void CglicTransform::setQuaternion(glm::quat Q){
  quat = Q;
  rot  = glm::toMat4(quat);
}
*/
void CglicTransform::reset(){
  tr   = glm::vec3(0.0f);
  rot  = glm::mat4(1.0f);
}

void CglicTransform::print()
{
  cout << "rotation:" << endl;
  for (int i = 0; i < 4; i++){
    for (int j = 0; j < 4; j++)
      cout << rot[i][j] << "  ";
    cout << endl;
  }
}
