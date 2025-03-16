#include <primitive.hpp>

namespace Terreate {
void Point::SetupBuffer() {
  Graphic::BufferDataConstructor bdc;
  bdc.AddVertexComponent("position", {{0, 0, 0}});
  bdc.SetVertexIndices({{0}});
  bdc.Construct();
  mBuffer.LoadData(bdc, {{"position", 0}});
  mBuffer.LoadIndices(Vec<Uint>({0}));
}

void Point::Draw(Resource<Renderer> &renderer) const {
  renderer->Bind();
  glPointSize(mPointSize);
  mBuffer.Draw(Graphic::DrawMode::POINTS);
  renderer->Unbind();
}

} // namespace Terreate
