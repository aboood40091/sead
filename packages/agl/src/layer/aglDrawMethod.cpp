#include <layer/aglDrawMethod.h>
#include <layer/aglRenderer.h>

namespace agl { namespace lyr {

DrawMethod::~DrawMethod()
{
    if (Renderer::instance())
        Renderer::instance()->removeDrawMethod(this);
}

} }
