// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "../precomp.hpp"

namespace cv
{

Vec4i calcVolumeDimentions(Point3i volumeResolution, bool ZFirstMemOrder);
int calcVolumeUnitDegree(Point3i volumeResolution);

class VolumeSettings::Impl
{
public:
    Impl() {};
    virtual ~Impl() {};

    virtual void  setWidth(int  val) = 0;
    virtual int   getWidth() const = 0;
    virtual void  setHeight(int  val) = 0;
    virtual int   getHeight() const = 0;
    virtual void  setDepthFactor(float val) = 0;
    virtual float getDepthFactor() const = 0;
    virtual void  setVoxelSize(float  val) = 0;
    virtual float getVoxelSize() const = 0;
    virtual void  setTruncatedDistance(float val) = 0;
    virtual float getTruncatedDistance() const = 0;
    virtual void  setTruncateThreshold(float val) = 0;
    virtual float getTruncateThreshold() const = 0;
    virtual void  setMaxWeight(int val) = 0;
    virtual int   getMaxWeight() const = 0;
    virtual void  setRaycastStepFactor(float val) = 0;
    virtual float getRaycastStepFactor() const = 0;
    virtual void  setZFirstMemOrder(bool val) = 0;
    virtual bool  getZFirstMemOrder() const = 0;
    virtual void  setVolumeUnitDegree(int val) = 0;
    virtual int   getVolumeUnitDegree() const = 0;

    virtual void setVolumePose(InputArray val) = 0;
    virtual void getVolumePose(OutputArray val) const = 0;
    virtual void setVolumeResolution(InputArray val) = 0;
    virtual void getVolumeResolution(OutputArray val) const = 0;
    virtual void setVolumeDimentions(InputArray val) = 0;
    virtual void getVolumeDimentions(OutputArray val) const = 0;
    virtual void setCameraIntrinsics(InputArray val) = 0;
    virtual void getCameraIntrinsics(OutputArray val) const = 0;
};

class VolumeSettingsImpl : public VolumeSettings::Impl
{
public:
    VolumeSettingsImpl();
    VolumeSettingsImpl(VolumeType volumeType);
    ~VolumeSettingsImpl();

    virtual void  setWidth(int  val) override;
    virtual int   getWidth() const override;
    virtual void  setHeight(int  val) override;
    virtual int   getHeight() const override;
    virtual void  setDepthFactor(float val) override;
    virtual float getDepthFactor() const override;
    virtual void  setVoxelSize(float  val) override;
    virtual float getVoxelSize() const override;
    virtual void  setTruncatedDistance(float val) override;
    virtual float getTruncatedDistance() const override;
    virtual void  setTruncateThreshold(float val) override;
    virtual float getTruncateThreshold() const override;
    virtual void  setMaxWeight(int val) override;
    virtual int   getMaxWeight() const override;
    virtual void  setRaycastStepFactor(float val) override;
    virtual float getRaycastStepFactor() const override;
    virtual void  setZFirstMemOrder(bool val) override;
    virtual bool  getZFirstMemOrder() const override;
    virtual void  setVolumeUnitDegree(int val) override;
    virtual int   getVolumeUnitDegree() const override;

    virtual void setVolumePose(InputArray val) override;
    virtual void getVolumePose(OutputArray val) const override;
    virtual void setVolumeResolution(InputArray val) override;
    virtual void getVolumeResolution(OutputArray val) const override;
    virtual void setVolumeDimentions(InputArray val) override;
    virtual void getVolumeDimentions(OutputArray val) const override;
    virtual void setCameraIntrinsics(InputArray val) override;
    virtual void getCameraIntrinsics(OutputArray val) const override;

private:
    VolumeType volumeType;

    int   width;
    int   height;
    float depthFactor;
    float voxelSize;
    float truncatedDistance;
    float truncateThreshold;
    int   maxWeight;
    float raycastStepFactor;
    bool  zFirstMemOrder;
    int volumeUnitDegree;

    Matx44f volumePose;
    Point3i volumeResolution;
    Vec4i volumeDimentions;
    Matx33f cameraIntrinsics;

public:
    // duplicate classes for all volumes

    class DefaultTsdfSets {
    public:
        static const int width  = 640;
        static const int height = 480;
        static constexpr float fx = 525.f; // focus point x axis
        static constexpr float fy = 525.f; // focus point y axis
        static constexpr float cx = float(width) / 2.f - 0.5f;  // central point x axis
        static constexpr float cy = float(height) / 2.f - 0.5f; // central point y axis
        static constexpr float depthFactor = 5000.f; // 5000 for the 16-bit PNG files, 1 for the 32-bit float images in the ROS bag files
        static constexpr float volumeSize = 3.f; // meters
        static constexpr float voxelSize = volumeSize / 128.f; //meters
        static constexpr float truncatedDistance = 2 * voxelSize;
        static constexpr float truncateThreshold = 0.f;
        static const int maxWeight = 64; // number of frames
        static constexpr float raycastStepFactor = 0.75f;
        static const bool zFirstMemOrder = true; // order of voxels in volume
        static const int volumeUnitDegree = 0;

        const Matx33f  cameraIntrinsics = Matx33f(fx, 0, cx, 0, fy, cy, 0, 0, 1); // camera settings
        const Affine3f volumePose = Affine3f().translate(Vec3f(-volumeSize / 2.f, -volumeSize / 2.f, 0.5f));
        const Matx44f  volumePoseMatrix = volumePose.matrix;
        // Unlike original code, this should work with any volume size
        // Not only when (x,y,z % 32) == 0
        const Point3i  volumeResolution = Vec3i::all(128); //number of voxels
    };

    class DefaultHashTsdfSets {
    public:
        static const int width = 640;
        static const int height = 480;
        static constexpr float fx = 525.f; // focus point x axis
        static constexpr float fy = 525.f; // focus point y axis
        static constexpr float cx = float(width) / 2.f - 0.5f;  // central point x axis
        static constexpr float cy = float(height) / 2.f - 0.5f; // central point y axis
        static constexpr float depthFactor = 5000.f; // 5000 for the 16-bit PNG files, 1 for the 32-bit float images in the ROS bag files
        static constexpr float volumeSize = 3.f; // meters
        static constexpr float voxelSize = volumeSize / 512.f; //meters
        static constexpr float truncatedDistance = 7 * voxelSize;
        static constexpr float truncateThreshold = 4.f;
        static const int maxWeight = 64; // number of frames
        static constexpr float raycastStepFactor = 0.25f;
        static const bool zFirstMemOrder = true; // order of voxels in volume

        const Matx33f  cameraIntrinsics = Matx33f(fx, 0, cx, 0, fy, cy, 0, 0, 1); // camera settings
        const Affine3f volumePose = Affine3f().translate(Vec3f(-volumeSize / 2.f, -volumeSize / 2.f, 0.5f));
        const Matx44f  volumePoseMatrix = volumePose.matrix;
        // Unlike original code, this should work with any volume size
        // Not only when (x,y,z % 32) == 0
        const Point3i  volumeResolution = Vec3i::all(16); //number of voxels
    };

};


VolumeSettings::VolumeSettings()
{
    this->impl = makePtr<VolumeSettingsImpl>();
}

VolumeSettings::VolumeSettings(VolumeType volumeType)
{
    this->impl = makePtr<VolumeSettingsImpl>(volumeType);
}

VolumeSettings::~VolumeSettings() {}

void  VolumeSettings::setWidth(int val) { this->impl->setWidth(val); };
int   VolumeSettings::getWidth() const { return this->impl->getWidth(); };
void  VolumeSettings::setHeight(int val) { this->impl->setHeight(val); };
int   VolumeSettings::getHeight() const { return this->impl->getHeight(); };
void  VolumeSettings::setVoxelSize(float val) { this->impl->setVoxelSize(val); };
float VolumeSettings::getVoxelSize() const { return this->impl->getVoxelSize(); };
void  VolumeSettings::setRaycastStepFactor(float val) { this->impl->setRaycastStepFactor(val); };
float VolumeSettings::getRaycastStepFactor() const { return this->impl->getRaycastStepFactor(); };
void  VolumeSettings::setTruncatedDistance(float val) { this->impl->setTruncatedDistance(val); };
float VolumeSettings::getTruncatedDistance() const { return this->impl->getTruncatedDistance(); };
void  VolumeSettings::setTruncateThreshold(float val) { this->impl->setTruncateThreshold(val); };
float VolumeSettings::getTruncateThreshold() const { return this->impl->getTruncateThreshold(); };
void  VolumeSettings::setDepthFactor(float val) { this->impl->setDepthFactor(val); };
float VolumeSettings::getDepthFactor() const { return this->impl->getDepthFactor(); };
void  VolumeSettings::setMaxWeight(int val) { this->impl->setMaxWeight(val); };
int   VolumeSettings::getMaxWeight() const { return this->impl->getMaxWeight(); };
void  VolumeSettings::setZFirstMemOrder(bool val) { this->impl->setZFirstMemOrder(val); };
bool  VolumeSettings::getZFirstMemOrder() const { return this->impl->getZFirstMemOrder(); };
void  VolumeSettings::setVolumeUnitDegree(int val) { this->impl->setVolumeUnitDegree(val); };
int   VolumeSettings::getVolumeUnitDegree() const { return this->impl->getVolumeUnitDegree(); };

void VolumeSettings::setVolumePose(InputArray val) { this->impl->setVolumePose(val); };
void VolumeSettings::getVolumePose(OutputArray val) const { this->impl->getVolumePose(val); };
void VolumeSettings::setVolumeResolution(InputArray val) { this->impl->setVolumeResolution(val); };
void VolumeSettings::getVolumeResolution(OutputArray val) const { this->impl->getVolumeResolution(val); };
void VolumeSettings::setVolumeDimentions(InputArray val) { this->impl->setVolumeDimentions(val); };
void VolumeSettings::getVolumeDimentions(OutputArray val) const { this->impl->getVolumeDimentions(val); };
void VolumeSettings::setCameraIntrinsics(InputArray val) { this->impl->setCameraIntrinsics(val); };
void VolumeSettings::getCameraIntrinsics(OutputArray val) const { this->impl->getCameraIntrinsics(val); };


VolumeSettingsImpl::VolumeSettingsImpl()
    : VolumeSettingsImpl(VolumeType::TSDF)
{
}

VolumeSettingsImpl::VolumeSettingsImpl(VolumeType _volumeType)
{
    volumeType = _volumeType;
    if (volumeType == VolumeType::TSDF)
    {
        DefaultTsdfSets ds = DefaultTsdfSets();

        this->width = ds.width;
        this->height = ds.height;
        this->depthFactor = ds.depthFactor;
        this->voxelSize = ds.voxelSize;
        this->truncatedDistance = ds.truncatedDistance;
        this->truncateThreshold = ds.truncateThreshold;
        this->maxWeight = ds.maxWeight;
        this->raycastStepFactor = ds.raycastStepFactor;
        this->zFirstMemOrder = ds.zFirstMemOrder;
        this->volumeUnitDegree = ds.volumeUnitDegree;

        this->volumePose = ds.volumePoseMatrix;
        this->volumeResolution = ds.volumeResolution;
        this->volumeDimentions = calcVolumeDimentions(ds.volumeResolution, ds.zFirstMemOrder);
        this->cameraIntrinsics = ds.cameraIntrinsics;
    }
    else if (volumeType == VolumeType::HashTSDF)
    {
        DefaultHashTsdfSets ds = DefaultHashTsdfSets();

        this->width = ds.width;
        this->height = ds.height;
        this->depthFactor = ds.depthFactor;
        this->voxelSize = ds.voxelSize;
        this->truncatedDistance = ds.truncatedDistance;
        this->truncateThreshold = ds.truncateThreshold;
        this->maxWeight = ds.maxWeight;
        this->raycastStepFactor = ds.raycastStepFactor;
        this->zFirstMemOrder = ds.zFirstMemOrder;
        this->volumeUnitDegree = calcVolumeUnitDegree(ds.volumeResolution);

        this->volumePose = ds.volumePoseMatrix;
        this->volumeResolution = ds.volumeResolution;
        this->volumeDimentions = calcVolumeDimentions(ds.volumeResolution, ds.zFirstMemOrder);
        this->cameraIntrinsics = ds.cameraIntrinsics;
    }

}


VolumeSettingsImpl::~VolumeSettingsImpl() {}


void VolumeSettingsImpl::setWidth(int val)
{
    this->width = val;
}

int VolumeSettingsImpl::getWidth() const
{
    return this->width;
}

void VolumeSettingsImpl::setHeight(int val)
{
    this->height = val;
}

int VolumeSettingsImpl::getHeight() const
{
    return this->height;
}

void VolumeSettingsImpl::setDepthFactor(float val)
{
    this->depthFactor = val;
}

float VolumeSettingsImpl::getDepthFactor() const
{
    return this->depthFactor;
}

void VolumeSettingsImpl::setVoxelSize(float  val)
{
    this->voxelSize = val;
}

float VolumeSettingsImpl::getVoxelSize() const
{
    return this->voxelSize;
}

void VolumeSettingsImpl::setTruncatedDistance(float val)
{
    this->truncatedDistance = val;
}

float VolumeSettingsImpl::getTruncatedDistance() const
{
    return this->truncatedDistance;
}

void VolumeSettingsImpl::setTruncateThreshold(float val)
{
    this->truncateThreshold = val;
}

float VolumeSettingsImpl::getTruncateThreshold() const
{
    return this->truncateThreshold;
}

void VolumeSettingsImpl::setMaxWeight(int val)
{
    this->maxWeight = val;
}

int VolumeSettingsImpl::getMaxWeight() const
{
    return this->maxWeight;
}

void VolumeSettingsImpl::setRaycastStepFactor(float val)
{
    this->raycastStepFactor = val;
}

float VolumeSettingsImpl::getRaycastStepFactor() const
{
    return this->raycastStepFactor;
}

void VolumeSettingsImpl::setZFirstMemOrder(bool val)
{
    this->zFirstMemOrder = val;
}

bool VolumeSettingsImpl::getZFirstMemOrder() const
{
    return this->zFirstMemOrder;
}

void VolumeSettingsImpl::setVolumeUnitDegree(int val)
{
    this->volumeUnitDegree = val;
}

int VolumeSettingsImpl::getVolumeUnitDegree() const
{
    return this->volumeUnitDegree;
}


void VolumeSettingsImpl::setVolumePose(InputArray val)
{
    if (!val.empty())
    {
        val.copyTo(this->volumePose);
    }
}

void VolumeSettingsImpl::getVolumePose(OutputArray val) const
{
    Mat(this->volumePose).copyTo(val);
}

void VolumeSettingsImpl::setVolumeResolution(InputArray val)
{
    if (!val.empty())
    {
        this->volumeResolution = Point3i(val.getMat());
    }
}

void VolumeSettingsImpl::getVolumeResolution(OutputArray val) const
{
    Mat(this->volumeResolution).copyTo(val);
}

void VolumeSettingsImpl::setVolumeDimentions(InputArray val)
{
    if (!val.empty())
    {
        this->volumeDimentions = Vec4i(val.getMat());
    }
}

void VolumeSettingsImpl::getVolumeDimentions(OutputArray val) const
{
    Mat(this->volumeDimentions).copyTo(val);
}

void VolumeSettingsImpl::setCameraIntrinsics(InputArray val)
{
    if (!val.empty())
    {
        this->cameraIntrinsics = Matx33f(val.getMat());
    }
}

void VolumeSettingsImpl::getCameraIntrinsics(OutputArray val) const
{
    Mat(this->cameraIntrinsics).copyTo(val);
}


Vec4i calcVolumeDimentions(Point3i volumeResolution, bool ZFirstMemOrder)
{
    // (xRes*yRes*zRes) array
    // Depending on zFirstMemOrder arg:
    // &elem(x, y, z) = data + x*zRes*yRes + y*zRes + z;
    // &elem(x, y, z) = data + x + y*xRes + z*xRes*yRes;
    int xdim, ydim, zdim;
    if (ZFirstMemOrder)
    {
        xdim = volumeResolution.z * volumeResolution.y;
        ydim = volumeResolution.z;
        zdim = 1;
    }
    else
    {
        xdim = 1;
        ydim = volumeResolution.x;
        zdim = volumeResolution.x * volumeResolution.y;
    }
    return Vec4i(xdim, ydim, zdim);
}


int calcVolumeUnitDegree(Point3i volumeResolution)
{
    if (!(volumeResolution.x & (volumeResolution.x - 1)))
    {
        // vuRes is a power of 2, let's get this power
        return trailingZeros32(volumeResolution.x);
    }
    else
    {
        CV_Error(Error::StsBadArg, "Volume unit resolution should be a power of 2");
    }
}


}
