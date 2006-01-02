//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Declaration of game document class
//
// This file is part of Gambit
// Copyright (c) 2005, The Gambit Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

#ifndef GAMEDOC_H
#define GAMEDOC_H

#include "libgambit/libgambit.h"
#include "style.h"
#include "analysis.h"

class gbtGameView;
class gbtGameDocument;

//!
//! This class manages the "stack" of supports obtained by eliminating
//! dominated actions from consideration.
//!
class gbtBehavDominanceStack {
private:
  gbtGameDocument *m_doc;
  bool m_strict;
  gbtArray<gbtEfgSupport *> m_supports;
  int m_current;
  bool m_noFurther;

public:
  gbtBehavDominanceStack(gbtGameDocument *p_doc, bool p_strict);
  ~gbtBehavDominanceStack();

  //!
  //! Returns the number of supports in the stack
  //!
  int NumSupports(void) const { return m_supports.Length(); }

  //!
  //! Get the i'th support in the stack
  //! (where i=1 is always the "full" support)
  //!
  const gbtEfgSupport &GetSupport(int i) const { return *m_supports[i]; }

  //!
  //! Get the current support
  //!
  const gbtEfgSupport &GetCurrent(void) const { return *m_supports[m_current]; }

  //!
  //! Get the level of iteration (1 = no iteration)
  //!
  int GetLevel(void) const { return m_current; }

  //!
  //! Sets whether elimination is strict or weak.  If this changes the
  //! internal setting, a Reset() is done
  //!
  void SetStrict(bool p_strict);

  //!
  //! Reset the stack by clearing out all supports
  //!
  void Reset(void);

  //!
  //! Go to the next level of iteration.  Returns 'true' if successful,
  //! 'false' if no effect (i.e., no further actions could be eliminated)
  //!
  bool NextLevel(void);

  //!
  //! Go to the previous level of iteration.  Returns 'true' if successful,
  //! 'false' if no effect (i.e., already at the full support)
  //!
  bool PreviousLevel(void);

  //!
  //! Go to the top level (the full support)
  //!
  void TopLevel(void) { m_current = 1; }

  //!
  //! Returns 'false' if it is known that no further eliminations can be done
  //!
  bool CanEliminate(void) const 
    { return (m_current < m_supports.Length() || !m_noFurther); }
};

//!
//! This class manages the "stack" of supports obtained by eliminating
//! dominated strategies from consideration.
//!
class gbtStrategyDominanceStack {
private:
  gbtGameDocument *m_doc;
  bool m_strict;
  gbtArray<gbtNfgSupport *> m_supports;
  int m_current;
  bool m_noFurther;

public:
  gbtStrategyDominanceStack(gbtGameDocument *p_doc, bool p_strict);
  ~gbtStrategyDominanceStack();

  //!
  //! Returns the number of supports in the stack
  //!
  int NumSupports(void) const { return m_supports.Length(); }

  //!
  //! Get the i'th support in the stack
  //! (where i=1 is always the "full" support)
  //!
  const gbtNfgSupport &GetSupport(int i) const { return *m_supports[i]; }

  //!
  //! Get the current support
  //!
  const gbtNfgSupport &GetCurrent(void) const { return *m_supports[m_current]; }

  //!
  //! Get the level of iteration (1 = no iteration)
  //!
  int GetLevel(void) const { return m_current; }

  //!
  //! Sets whether elimination is strict or weak.  If this changes the
  //! internal setting, a Reset() is done
  //!
  void SetStrict(bool p_strict);

  //!
  //! Gets whether elimination is strict or weak.
  //!
  bool GetStrict(void) const { return m_strict; }

  //!
  //! Reset the stack by clearing out all supports
  //!
  void Reset(void);

  //!
  //! Go to the next level of iteration.  Returns 'true' if successful,
  //! 'false' if no effect (i.e., no further actions could be eliminated)
  //!
  bool NextLevel(void);

  //!
  //! Go to the previous level of iteration.  Returns 'true' if successful,
  //! 'false' if no effect (i.e., already at the full support)
  //!
  bool PreviousLevel(void);

  //!
  //! Go to the top level (the full support)
  //!
  void TopLevel(void) { m_current = 1; }

  //!
  //! Returns 'false' if it is known that no further eliminations can be done
  //!
  bool CanEliminate(void) const 
    { return (m_current < m_supports.Length() || !m_noFurther); }
	      
};


//
// These are passed to gbtGameDocument::UpdateViews() to indicate which
// types of modifications have occurred.
//
// GBT_DOC_MODIFIED_GAME: The game itself has been modified; that is, the
// physical structure (number of players, number of strategies, game tree)
// has changed.  This requires all computed data to be deleted.
//
// GBT_DOC_MODIFIED_PAYOFFS: The payoffs of the game have changed, but not
// the physical structure.  This means that strategy profiles can, in
// principle, be kept -- but they may no longer be Nash, etc.
//
// GBT_DOC_MODIFIED_LABELS: Labeling of players, strategies, etc. has
// changed.  These have no effect on the game mathematically, so computed
// data may be kept; but, we want to track the label change for undo, etc.
//
// GBT_DOC_MODIFIED_VIEWS: Information about how the document is viewed
// (e.g., player colors) has changed.  We want to track this for undo,
// but, again, this has no effect on the game mathematically.
// 
typedef enum {
  GBT_DOC_MODIFIED_NONE = 0x00,
  GBT_DOC_MODIFIED_GAME = 0x01,
  GBT_DOC_MODIFIED_PAYOFFS = 0x02,
  GBT_DOC_MODIFIED_LABELS = 0x04,
  GBT_DOC_MODIFIED_VIEWS = 0x08
} gbtGameModificationType;

class gbtGameDocument {
friend class gbtGameView;
private:
  gbtArray<gbtGameView *> m_views;

  void AddView(gbtGameView *p_view)  { m_views.Append(p_view); }
  void RemoveView(gbtGameView *p_view)
    { 
      m_views.Remove(m_views.Find(p_view)); 
      if (m_views.Length() == 0)  delete this;
    }

  Gambit::GameTree m_efg;
  Gambit::GameTable m_nfg;
  wxString m_filename;

  gbtStyle m_style;
  Gambit::GameNode m_selectNode;
  bool m_modified;

  gbtBehavDominanceStack m_behavSupports;
  gbtStrategyDominanceStack m_stratSupports;

  gbtList<gbtAnalysisProfileList> m_profiles;
  int m_currentProfileList;

  gbtList<std::string> m_undoList, m_redoList;

public:
  gbtGameDocument(Gambit::GameTree p_efg);
  gbtGameDocument(Gambit::GameTable p_nfg); 
  
  ~gbtGameDocument();

  //!
  //! @name Reading and writing .gbt savefiles
  //!
  //@{
  /// Load document from the specified file (which should be a .gbt file)
  /// Returns true if successful, false if error
  bool LoadDocument(const wxString &p_filename, bool p_saveUndo = true);
  void SaveDocument(std::ostream &) const;
  //@}

  Gambit::GameTree GetEfg(void) const { return m_efg; }
  Gambit::GameTable GetNfg(void) const 
    { 
      if (m_efg) return m_efg->AssociatedNfg();
      else return m_nfg;
    }

  void BuildNfg(void);

  const wxString &GetFilename(void) const { return m_filename; }
  void SetFilename(const wxString &p_filename) { m_filename = p_filename; }

  bool IsModified(void) const { return m_modified; }
  void SetModified(bool p_modified) { m_modified = p_modified; }

  gbtStyle &GetStyle(void) { return m_style; }

  int NumPlayers(void) const
    { return (m_efg) ? m_efg->NumPlayers() : m_nfg->NumPlayers(); }
  bool IsConstSum(void) const
    { return ((m_efg && m_efg->IsConstSum()) || 
	      (!m_efg && m_nfg->IsConstSum())); }


  //!
  //! @name Handling of undo/redo features
  //!
  //@{
  bool CanUndo(void) const { return (m_undoList.Length() > 1); }
  void Undo(void);

  bool CanRedo(void) const { return (m_redoList.Length() > 0); }
  void Redo(void);
  //@}

  //!
  //! @name Handling of list of computed profiles
  //!
  //@{
  const gbtAnalysisProfileList &GetProfiles(void) const
    { return m_profiles[m_currentProfileList]; }
  const gbtAnalysisProfileList &GetProfiles(int p_index) const
    { return m_profiles[p_index]; }
  void AddProfileList(void);
  void AddProfileList(const gbtAnalysisProfileList &);
  void SetProfileList(int p_index);
  int NumProfileLists(void) const { return m_profiles.Length(); }
  int GetCurrentProfileList(void) const { return m_currentProfileList; }

  int GetCurrentProfile(void) const 
  { return (m_profiles.Length() == 0) ? 0 : GetProfiles().GetCurrent(); }
  void SetCurrentProfile(int p_profile);
  void AddProfiles(const gbtList<gbtBehavProfile<gbtNumber> > &);
  void AddProfile(const gbtBehavProfile<gbtNumber> &);
  void AddProfiles(const gbtList<gbtMixedProfile<gbtNumber> > &);
  void AddProfile(const gbtMixedProfile<gbtNumber> &);
  //@}

  //!
  //! @name Handling of behavior supports
  //!
  //@{
  const gbtEfgSupport &GetEfgSupport(void) const
    { return m_behavSupports.GetCurrent(); }
  void SetBehavElimStrength(bool p_strict);
  bool NextBehavElimLevel(void);
  void PreviousBehavElimLevel(void);
  void TopBehavElimLevel(void);
  bool CanBehavElim(void) const;
  int GetBehavElimLevel(void) const;
  //@}

  //!
  //! @name Handling of strategy supports
  //!
  //@{
  const gbtNfgSupport &GetNfgSupport(void) const
    { return m_stratSupports.GetCurrent(); }
  void SetStrategyElimStrength(bool p_strict);
  bool GetStrategyElimStrength(void) const;
  bool NextStrategyElimLevel(void);
  void PreviousStrategyElimLevel(void);
  void TopStrategyElimLevel(void);
  bool CanStrategyElim(void) const;
  int GetStrategyElimLevel(void) const;
  //@}

  Gambit::GameNode GetSelectNode(void) const { return m_selectNode; }
  void SetSelectNode(Gambit::GameNode);

  std::string GetRealizProb(const Gambit::GameNode &) const;
  std::string GetBeliefProb(const Gambit::GameNode &) const;
  std::string GetNodeValue(const Gambit::GameNode &, int pl) const;
  std::string GetInfosetProb(const Gambit::GameNode &) const;
  std::string GetInfosetValue(const Gambit::GameNode &) const;
  std::string GetActionValue(const Gambit::GameNode &, int act) const;
  std::string GetActionProb(const Gambit::GameNode &, int act) const;
  gbtNumber ActionProb(const Gambit::GameNode &n, int br) const;

  void UpdateViews(gbtGameModificationType p_modifications);

  /// Call to ask viewers to post any pending changes
  void PostPendingChanges(void);
};

class gbtGameView {
protected:
  gbtGameDocument *m_doc;

public:
  gbtGameView(gbtGameDocument *p_doc)
    : m_doc(p_doc) { m_doc->AddView(this); }
  virtual ~gbtGameView()
    { m_doc->RemoveView(this); }

  virtual void OnUpdate(void) = 0;

  /// Post any pending changes in the viewer to the document
  virtual void PostPendingChanges(void) { }

  gbtGameDocument *GetDocument(void) const { return m_doc; }
};

#endif  // GAMEDOC_H
