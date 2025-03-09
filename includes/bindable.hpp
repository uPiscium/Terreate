#ifndef __TERREATE_GRAPHIC_BINDABLE_HPP__
#define __TERREATE_GRAPHIC_BINDABLE_HPP__

namespace Terreate::Interface {

struct IBindable {
public:
  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;
};

} // namespace Terreate::Interface

#endif // __TERREATE_GRAPHIC_BINDABLE_HPP__
