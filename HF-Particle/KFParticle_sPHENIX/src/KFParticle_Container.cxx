/*****************/
/* Cameron Dean  */
/*   LANL 2020   */
/* cdean@bnl.gov */
/*****************/

//Ideas taken from SvtxTrackMap_v1 & TrkrClusterContainer 

#include <KFParticle_Container.h>
#include "KFParticle.h"

#include <phool/PHObject.h>  // for PHObject

#include <iterator>     // for reverse_iterator
#include <map>          // for _Rb_tree_const_iterator, _Rb_tree_iterator
#include <ostream>      // for operator<<, endl, ostream, basic_ostream, bas...
#include <utility>      // for pair, make_pair

using namespace std;

KFParticle_Container::KFParticle_Container()
  : m_kfpmap()
{
}

KFParticle_Container::KFParticle_Container(const KFParticle_Container& kfparticlemap)
  : m_kfpmap()
{
  for (ConstIter iter = kfparticlemap.begin();
       iter != kfparticlemap.end();
       ++iter)
  {
    KFParticle* particle = dynamic_cast<KFParticle*> (iter->second->Clone());
    m_kfpmap.insert(make_pair(particle->Id(), particle)); 
  }
}

KFParticle_Container& KFParticle_Container::operator=(const KFParticle_Container& kfparticlemap)
{
  Reset();
  for (ConstIter iter = kfparticlemap.begin();
       iter != kfparticlemap.end();
       ++iter)
  {
    KFParticle* particle = dynamic_cast<KFParticle*> (iter->second->Clone());
    m_kfpmap.insert(make_pair(particle->Id(), particle));
  }
  return *this;
}

KFParticle_Container::~KFParticle_Container()
{
  Reset();
}

void KFParticle_Container::Reset()
{
  for (Iter iter = m_kfpmap.begin();
       iter != m_kfpmap.end();
       ++iter)
  {
    KFParticle* particle = iter->second;
    delete particle;
  }
  m_kfpmap.clear();
}

void KFParticle_Container::identify(ostream& os) const
{
  os << "KFParticle_Container: size = " << m_kfpmap.size() << endl;
  return;
}

const KFParticle* KFParticle_Container::get(unsigned int id) const
{
  ConstIter iter = m_kfpmap.find(id);
  if (iter == m_kfpmap.end()) return nullptr;
  return iter->second;
}

KFParticle* KFParticle_Container::get(unsigned int id)
{
  Iter iter = m_kfpmap.find(id);
  if (iter == m_kfpmap.end()) return nullptr;
  return iter->second;
}

KFParticle* KFParticle_Container::insert(const KFParticle* particle)
{
  unsigned int index = 0;
  if (!m_kfpmap.empty()) index = m_kfpmap.rbegin()->first + 1;
  m_kfpmap.insert(make_pair(index, dynamic_cast<KFParticle*> (particle->Clone())));
  m_kfpmap[index]->SetId(index);
  return m_kfpmap[index];
}

KFParticle_Container::ConstIter
KFParticle_Container::addParticle(KFParticle* particle)
{
  return addParticleSpecifyKey(particle->Id(), particle);
}

KFParticle_Container::ConstIter
KFParticle_Container::addParticleSpecifyKey(unsigned int id, KFParticle* particle)
{
  auto ret = m_kfpmap.insert(make_pair(id, particle));
  if ( !ret.second )
  {
    cout << "KFParticle_Container::AddParticleSpecifyKey: duplicate id: " << id << " exiting now" << endl;
    exit(1);
  }
  else
  {
    return ret.first;
  }
}


KFParticle_Container::Map
KFParticle_Container::returnParticlesByPDGid( int PDGid )
{
  Map requiredParticles;

  for (Iter iter = m_kfpmap.begin(); iter != m_kfpmap.end(); ++iter)
    if (iter->second->GetPDG() == PDGid) 
      requiredParticles.insert(make_pair(iter->first, iter->second));

  return requiredParticles;
}
