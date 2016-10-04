/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: Handle.h 
 * Author: Judy Cheng, (Credit Zachary Nawar)
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma once
#include "Base\util\file\File.h"
#include "Base\modules\introspection\variable\Variable.h"

namespace Base
{
  class Handle
  {
  public:
    Handle();
    Handle(unsigned index, unsigned counter);
    Handle(unsigned handle);

    static void Serialize(File& file, Variable var);

    inline operator unsigned(void) const;

    unsigned m_index : 16;
    unsigned m_counter : 16;

    static const Handle null;
  };

  Handle::operator unsigned( void ) const
  {
    return m_counter << 16 | m_index;
  }

}
