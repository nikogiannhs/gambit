//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Interface to player representation classes
//
// This file is part of Gambit
// Copyright (c) 2002, The Gambit Project
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

#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"

//
// The extensive form and normal form player classes are both included
// in this header file in the hopes of an eventual unification of the
// two.
//



class gbtGamePlayerBase;
class gbtGame;
class gbtGameInfoset;

class gbtGamePlayer;
class gbtGamePlayer;
class gbtGameAction;

class gbtGameStrategyRep : public gbtGameObject {
friend class gbtGameStrategy;
public:
  virtual gbtText GetLabel(void) const = 0;
  virtual void SetLabel(const gbtText &) = 0;
  virtual int GetId(void) const = 0;

  virtual gbtGamePlayer GetPlayer(void) const = 0;
  virtual gbtGameAction GetAction(const gbtGameInfoset &) const = 0;
  virtual const gbtArray<int> &GetBehavior(void) const = 0;

  // The following only make sense for matrix games
  virtual long GetIndex(void) const = 0;
};

class gbtEfgNullStrategy { };

class gbtGameStrategy {
friend class gbtGame;
private:
  gbtGameStrategyRep *m_rep;

public:
  gbtGameStrategy(void) : m_rep(0) { }
  gbtGameStrategy(gbtGameStrategyRep *p_rep)
    : m_rep(p_rep) { if (m_rep) m_rep->Reference(); }
  gbtGameStrategy(const gbtGameStrategy &p_player)
    : m_rep(p_player.m_rep) { if (m_rep) m_rep->Reference(); }
  ~gbtGameStrategy() { if (m_rep && m_rep->Dereference()) delete m_rep; }

  gbtGameStrategy &operator=(const gbtGameStrategy &p_player) {
    if (this != &p_player) {
      if (m_rep && m_rep->Dereference()) delete m_rep;
      m_rep = p_player.m_rep;
      if (m_rep) m_rep->Reference();
    }
    return *this;
  }

  bool operator==(const gbtGameStrategy &p_player) const
  { return (m_rep == p_player.m_rep); }
  bool operator!=(const gbtGameStrategy &p_player) const
  { return (m_rep != p_player.m_rep); }

  gbtGameStrategyRep *operator->(void) 
  { if (!m_rep) throw gbtEfgNullStrategy(); return m_rep; }
  const gbtGameStrategyRep *operator->(void) const 
  { if (!m_rep) throw gbtEfgNullStrategy(); return m_rep; }
  
  gbtGameStrategyRep *Get(void) const { return m_rep; }

  // Questionable whether this should be provided
  bool IsNull(void) const { return (m_rep == 0); }
};

inline gbtOutput &operator<<(gbtOutput &p_stream,
			     const gbtGameStrategy &)
{ return p_stream; }

class gbtGamePlayerRep : public gbtGameObject {
friend class gbtGamePlayer;
friend class gbtGame;
friend class gbtGameInfoset;
public:
  virtual gbtGame GetGame(void) const = 0;
  virtual gbtText GetLabel(void) const = 0;
  virtual void SetLabel(const gbtText &) = 0;
  virtual int GetId(void) const = 0;

  virtual bool IsChance(void) const = 0;

  virtual int NumInfosets(void) const = 0;
  virtual gbtGameInfoset NewInfoset(int p_actions) = 0;
  virtual gbtGameInfoset GetInfoset(int p_index) const = 0;

  virtual int NumStrategies(void) const = 0;
  virtual gbtGameStrategy GetStrategy(int p_index) const = 0;
};

class gbtGameNullPlayer { };

class gbtGamePlayer {
private:
  gbtGamePlayerRep *m_rep;

public:
  gbtGamePlayer(void) : m_rep(0) { }
  gbtGamePlayer(gbtGamePlayerRep *p_rep)
    : m_rep(p_rep) { if (m_rep) m_rep->Reference(); }
  gbtGamePlayer(const gbtGamePlayer &p_player)
    : m_rep(p_player.m_rep) { if (m_rep) m_rep->Reference(); }
  ~gbtGamePlayer() { if (m_rep && m_rep->Dereference()) delete m_rep; }

  gbtGamePlayer &operator=(const gbtGamePlayer &p_player) {
    if (this != &p_player) {
      if (m_rep && m_rep->Dereference()) delete m_rep;
      m_rep = p_player.m_rep;
      if (m_rep) m_rep->Reference();
    }
    return *this;
  }

  bool operator==(const gbtGamePlayer &p_player) const
  { return (m_rep == p_player.m_rep); }
  bool operator!=(const gbtGamePlayer &p_player) const
  { return (m_rep != p_player.m_rep); }

  gbtGamePlayerRep *operator->(void) 
  { if (!m_rep) throw gbtGameNullPlayer(); return m_rep; }
  const gbtGamePlayerRep *operator->(void) const 
  { if (!m_rep) throw gbtGameNullPlayer(); return m_rep; }
  
  gbtGamePlayerRep *Get(void) const { return m_rep; }

  // Questionable whether this should be provided
  bool IsNull(void) const { return (m_rep == 0); }
};

#endif  // PLAYER_H
