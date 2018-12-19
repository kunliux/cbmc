/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include "dplib_prop.h"

#include <cassert>

#include <set>

dplib_propt::dplib_propt(std::ostream &_out):out(_out)
{
  // we skip index 0
  _no_variables=1;
}

void dplib_propt::land(literalt a, literalt b, literalt o)
{
  out << "// land\n";
  out << "AXIOM (" << dplib_literal(a) << " & "
      << dplib_literal(b) << ") <=> " << dplib_literal(o)
      << ";\n\n";
}

void dplib_propt::lor(literalt a, literalt b, literalt o)
{
  out << "// lor\n";
  out << "AXIOM (" << dplib_literal(a) << " | "
      << dplib_literal(b) << ") <=> " << dplib_literal(o)
      << ";\n\n";
}

void dplib_propt::lxor(literalt a, literalt b, literalt o)
{
  out << "// lxor\n";
  out << "AXIOM (" << dplib_literal(a) << " <=> "
      << dplib_literal(b) << ") <=> !" << dplib_literal(o)
      << ";\n\n";
}

void dplib_propt::lnand(literalt a, literalt b, literalt o)
{
  out << "// lnand\n";
  out << "AXIOM (" << dplib_literal(a) << " & "
      << dplib_literal(b) << ") <=> !" << dplib_literal(o)
      << ";\n\n";
}

void dplib_propt::lnor(literalt a, literalt b, literalt o)
{
  out << "// lnor\n";
  out << "AXIOM (" << dplib_literal(a) << " | "
      << dplib_literal(b) << ") <=> !" << dplib_literal(o)
      << ";\n\n";
}

void dplib_propt::lequal(literalt a, literalt b, literalt o)
{
  out << "// lequal\n";
  out << "AXIOM (" << dplib_literal(a) << " <=> "
      << dplib_literal(b) << ") <=> " << dplib_literal(o)
      << ";\n\n";
}

void dplib_propt::limplies(literalt a, literalt b, literalt o)
{
  out << "// limplies\n";
  out << "AXIOM (" << dplib_literal(a) << " => "
      << dplib_literal(b) << ") <=> " << dplib_literal(o)
      << ";\n\n";
}

literalt dplib_propt::land(const bvt &bv)
{
  out << "// land\n";

  literalt literal=def_dplib_literal();

  forall_literals(it, bv)
  {
    if(it!=bv.begin())
      out << " & ";
    out << dplib_literal(*it);
  }

  out << "\n\n";

  return literal;
}

literalt dplib_propt::lor(const bvt &bv)
{
  out << "// lor\n";

  literalt literal=def_dplib_literal();

  forall_literals(it, bv)
  {
    if(it!=bv.begin())
      out << " | ";
    out << dplib_literal(*it);
  }

  out << "\n\n";

  return literal;
}

literalt dplib_propt::lxor(const bvt &bv)
{
  if(bv.empty())
    return const_literal(false);
  if(bv.size()==1)
    return bv[0];
  if(bv.size()==2)
    return lxor(bv[0], bv[1]);

  literalt literal=const_literal(false);

  forall_literals(it, bv)
    literal=lxor(*it, literal);

  return literal;
}

literalt dplib_propt::land(literalt a, literalt b)
{
  if(a==const_literal(true))
    return b;
  if(b==const_literal(true))
    return a;
  if(a==const_literal(false))
    return const_literal(false);
  if(b==const_literal(false))
    return const_literal(false);
  if(a==b)
    return a;

  literalt o=def_dplib_literal();
  out << dplib_literal(a) << " & " << dplib_literal(b)
      << ";\n\n";

  return o;
}

literalt dplib_propt::lor(literalt a, literalt b)
{
  if(a==const_literal(false))
    return b;
  if(b==const_literal(false))
    return a;
  if(a==const_literal(true))
    return const_literal(true);
  if(b==const_literal(true))
    return const_literal(true);
  if(a==b)
    return a;

  literalt o=def_dplib_literal();
  out << dplib_literal(a) << " | " << dplib_literal(b)
      << ";\n\n";

  return o;
}

literalt dplib_propt::lxor(literalt a, literalt b)
{
  if(a==const_literal(false))
    return b;
  if(b==const_literal(false))
    return a;
  if(a==const_literal(true))
    return !b;
  if(b==const_literal(true))
    return !a;

  literalt o=def_dplib_literal();
  out << "!(" << dplib_literal(a) << " <-> " << dplib_literal(b)
      << ");\n\n";

  return o;
}

literalt dplib_propt::lnand(literalt a, literalt b)
{
  return !land(a, b);
}

literalt dplib_propt::lnor(literalt a, literalt b)
{
  return !lor(a, b);
}

literalt dplib_propt::lequal(literalt a, literalt b)
{
  return !lxor(a, b);
}

literalt dplib_propt::limplies(literalt a, literalt b)
{
  return lor(!a, b);
}

literalt dplib_propt::lselect(literalt a, literalt b, literalt c)
{
  if(a==const_literal(true))
    return b;
  if(a==const_literal(false))
    return c;
  if(b==c)
    return b;

  out << "// lselect\n";

  literalt o=def_dplib_literal();

  out << "IF " << dplib_literal(a) << " THEN "
      << dplib_literal(b) << " ELSE "
      << dplib_literal(c) << " ENDIF;\n\n";

  return o;
}

literalt dplib_propt::new_variable()
{
  _no_variables++;
  out << "l" << _no_variables << ": boolean;\n";
  literalt l;
  l.set(_no_variables, false);
  return l;
}

literalt dplib_propt::def_dplib_literal()
{
  _no_variables++;
  out << "l" << _no_variables << ": boolean = ";
  literalt l;
  l.set(_no_variables, false);
  return l;
}

void dplib_propt::lcnf(const bvt &bv)
{
  if(bv.empty())
    return;
  bvt new_bv;

  std::set<literalt> s;

  new_bv.reserve(bv.size());

  for(bvt::const_iterator it=bv.begin(); it!=bv.end(); it++)
  {
    if(s.insert(*it).second)
      new_bv.push_back(*it);

    if(s.find(!*it)!=s.end())
      return; // clause satisfied

    assert(it->var_no()<=_no_variables);
  }

  assert(!new_bv.empty());

  out << "// lcnf\n";
  out << "AXIOM ";

  for(bvt::const_iterator it=new_bv.begin(); it!=new_bv.end(); it++)
  {
    if(it!=new_bv.begin())
      out << " | ";
    out << dplib_literal(*it);
  }

  out << ";\n\n";
}

std::string dplib_propt::dplib_literal(literalt l)
{
  if(l==const_literal(false))
    return "FALSE";
  else if(l==const_literal(true))
    return "TRUE";

  if(l.sign())
    return "(NOT l"+std::to_string(l.var_no())+")";

  return "l"+std::to_string(l.var_no());
}

void dplib_propt::finish()
{
  // we want satisfiability
  out << "THEOREM false;\n";
}

propt::resultt dplib_propt::prop_solve()
{
  finish();
  return P_ERROR;
}
