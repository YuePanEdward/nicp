#pragma once

#include <tr1/memory>
#include <stack>
#include <set>

#include "boss/eigen_boss_plugin.h"
#include "globals/defs.h"
#include "globals/draw_attributes.h"

namespace nicp {

  class MapNode;
  typedef std::set<MapNode*> MapNodeSet;

  // this class defines a node in a multilevel map
  class MapNode: public nicp::Identifiable{
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    typedef std::stack<Eigen::Isometry3f, std::vector<Eigen::Isometry3f,  Eigen::aligned_allocator<Eigen::Isometry3f> > > BackupStackType;

    MapNode(const Eigen::Isometry3f& transform=Eigen::Isometry3f::Identity(),
		       int id=-1,
		       nicp::IdContext* context=0);

    inline const Eigen::Isometry3f& transform() const {return _transform;}
    inline void setTransform(const Eigen::Isometry3f& t) {_transform = t;}
    inline double timestamp() const {return _timestamp;}
    inline void setTimestamp(double ts)  { _timestamp = ts;}

    virtual void draw(DrawAttributesType attributes = ATTRIBUTE_SHOW, int name=-1);
    virtual ~MapNode();
    
    virtual void serialize(nicp::ObjectData& data, nicp::IdContext& context);
    virtual void deserialize(nicp::ObjectData& data, nicp::IdContext& context);
    virtual void deserializeComplete();

    virtual void push() { _backup.push(_transform);}
    virtual void pop() { 
      if(!(_backup.size() > 0)) { return; }
      _transform = _backup.top();
      _backup.pop();
    }
    virtual void discardTop() {
      if(!(_backup.size() > 0)) { return; }
      _backup.pop();
    }
    virtual int stackSize() const { return _backup.size(); }

    const MapNodeSet& parents() const {return _parents;}
    MapNodeSet& parents() {return _parents;}

    inline std::vector<nicp::Identifiable*>& pendingParents() {return _pending_parents;}
    
  protected:
    Eigen::Isometry3f _transform;
    double _timestamp;
    MapNodeSet _parents;
    std::vector<nicp::Identifiable*> _pending_parents;
    BackupStackType _backup;
  };
}
