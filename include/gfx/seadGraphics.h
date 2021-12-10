#ifndef SEAD_GRAPHICS_H_
#define SEAD_GRAPHICS_H_

namespace sead {

class Graphics
{
public:
    enum DevicePosture
    {
        cDevicePosture_Same = 0,
        cDevicePosture_RotateRight = 1,
        cDevicePosture_RotateLeft = 2,
        cDevicePosture_RotateHalfAround = 3,
        cDevicePosture_FlipX = 4,
        cDevicePosture_FlipY = 5,
        cDevicePosture_FlipXY = 3,
        cDevicePosture_Invalid = 4,
    };
};

} // namespace sead

#endif // SEAD_GRAPHICS_H_
