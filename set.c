/**
 * @file set.c Schnittstelle einer Bibliothek fuer Mengenoperationen.
 *
 * Mengen werden intern durch sortierte verkettete Listen dargestellt.
 *
 * @author Christopher Blöcker
 */

#include <assert.h>
#include <stdio.h>

#include "list.h"
#include "set.h"

/* ########################################################################## */

/**
 * Berechnet die Summe aller Elemente der Menge s.
 *
 * @param [in] s Menge, von der die Summe der Elemente zu berechnen ist.
 *
 * @return Summe der Elemente der Menge s
 */
static Element set_sum(Set s)
{
  return (set_is_empty(s)) ? 0 : set_elementValue(set_get_first(s)) + set_sum(list_tail(s));
}

/**
 * Bestimmt die Partitionen der Menge s. Die Partitionen sind dadurch definiert,
 * dass die Summe der Elemente der ersten Partition gleich oder größer der Summe
 * der Elemente der zweiten Partition ist. Dabei ist das größte Element der ersten
 * Partition kleiner als das kleinste Element der zweiten Partition.
 *
 * @param [in,out] *res1 erste Partition
 * @param [in,out] *res2 zweite Partition
 * @param [in] s Menge, von der die Partitionen bestimmt werden sollen
 */
static void partition(Set *res1, Set *res2, Set s)
{
  if (!set_is_empty(s))
  {    
    if (set_sum(*res1) < set_sum(s))
    {
      set_insert(res1, set_get_first(s));
      partition(res1, res2, list_tail(s));
    }
    else
      set_copy(res2, s);
  }
}

/**
 * Gibt die Stringrepräsentation der Elemente der Menge s aus. 
 * Vor jedem der Elemente wird ein Komma ausgegeben.
 *
 * @param [in] s Menge, die ausgegeben werden soll
 */
static void printSet(Set s)
{
  if (!list_isEmpty(s))
  {
    fprintf(stdout, ",");
    set_printElement(set_get_first(s));
    printSet(list_tail(s));
  }
}

/* ########################################################################## */

/**
 * Initialisiert eine Menge als leere Menge.
 *
 * @param[out] s Zeiger auf die initialisierte Menge.
 */
void set_make_empty (Set * s)
{
  *s = list_mkEmpty();
}

/**
 * Fuegt ein Element e in eine Menge s ein.
 *
 * @param[in,out] s Zeiger auf die Menge.
 * @param[in] e das einzufuegende Element.
 *
 * @pre s ist Zeiger auf existierende Menge.
 * @post die Menge, auf die s zeigt, enthält den Wert e.
 */
void set_insert (Set * s, Element e)
{
  assert(s != NULL);
  
  *s = list_insertElem(set_copyElement(e), *s);
  
  assert(set_contains(*s, set_elementValue(e)));
}

/**
 * Loescht ein Element e aus einer Menge s.
 *
 * @param[in,out] s Zeiger auf die Menge.
 * @param[in] e das zu loeschende Element.
 *
 * @pre s ist Zeiger auf existierende Menge.
 * @post der Wert e ist nicht in der Menge, auf die s zeigt,
 *                  enthalten.
 */
void set_remove (Set * s, Element e)
{
  assert(s != NULL);

  *s = list_removeElem(set_elementValue(e), *s);

  assert(!set_contains(*s, set_elementValue(e)));
}

/**
 * Loescht alle Elemente aus einer Menge.
 *
 * @param[in,out] s Zeiger auf die Menge.
 *
 * @pre s ist Zeiger auf existierende Menge.
 * @post s ist Zeiger auf leere Menge.
 */
void set_remove_all_elems (Set * s)
{
  assert(s != NULL);

  *s = list_removeAllElems(*s);
  
  assert(set_is_empty(*s));
}

/**
 * Prueft, ob eine Menge leer ist.
 *
 * @param[in] s die Menge.
 *
 * @return 1, falls Menge leer ist; 0 sonst.
 */
int set_is_empty (Set s)
{
  return list_isEmpty(s);
}

/**
 * Liefert die Kardinalitaet einer Menge.
 *
 * @param[in] s die Menge.
 *
 * @return Kardinalitaet einer Menge.
 */
unsigned long set_cardinality (Set s)
{
  return list_length(s);
}

/**
 * Liefert das Element der Menge, das gemaess der Ordnungsrelation der Elemente
 * das erste in einer geordneten Liste der Elemente ist.
 *
 * @param[in] s die Menge.
 *
 * @return groesstes Element in der Menge.
 *
 * @pre s ist nicht leer.
 */
Element set_get_first (Set s)
{
  assert(!set_is_empty(s));

  return list_head(s);
}

/**
 * Liefert das Element der Menge, das gemaess der Ordnungsrelation der Elemente
 * das letzte in einer geordneten Liste der Elemente ist.
 *
 * @param[in] s die Menge.
 *
 * @return kleinstes Element in der Menge.
 *
 * @pre s ist nicht leer.
 */
Element set_get_last (Set s)
{
  assert(!set_is_empty(s));

  if (set_is_empty(list_tail(s))) return set_get_first(s);
  return set_get_last(list_tail(s));
}

/**
 * Prueft, ob ein Element in einer Menge enthalten ist.
 *
 * @param[in] s die Menge.
 * @param[in] e das zu pruefende Element.
 *
 * @return 1, falls Element enthalten ist; 0 sonst.
 */
int set_contains (Set s, Element e)
{
  return list_isIn(set_elementValue(e), s);
}

/**
 * Prueft, ob die Menge a Untermenge der Menge b ist.
 *
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @return 1, falls Menge a Untermenge von b ist; 0 sonst.
 */
int set_is_subset (Set a, Set b)
{
  if (set_is_empty(a)) return 1;
  if (set_contains(b, set_get_first(a))) return set_is_subset(list_tail(a), list_tail(b));
  return 0;
}

/**
 * Prueft, ob die Menge a gleich der Menge b ist.
 *
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @return 1, falls Menge a gleich Menge b ist; 0 sonst.
 */
int set_equals (Set a, Set b)
{
  if (set_is_empty(a) && set_is_empty(b)) return 1;
  if (set_is_empty(a) || set_is_empty(b)) return 0;
  if (list_eqElement(set_elementValue(set_get_first(a)), set_elementValue(set_get_first(b))))
    return set_equals(list_tail(a), list_tail(b));
  return 0;
}

/**
 * Erzeugt die Vereinigungsmenge zweier Mengen a und b.
 *
 * @param[in,out] res Zeiger auf die Vereinigungsmenge von a und b.
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @pre res zeigt auf existierende leere Menge.
 */
void set_union (Set * res, Set a, Set b)
{
  assert(set_is_empty(*res));
  
  if (!set_is_empty(a) && !set_is_empty(b))
  {
    if (!list_geElement(set_get_first(a), set_get_first(b)))
    {
      set_union(res, list_tail(a), b);
      *res = list_cons(set_copyElement(set_get_first(a)), *res);
    }
      
    else if (list_eqElement(set_get_first(a), set_get_first(b)))
    {
      set_union(res, list_tail(a), list_tail(b));
      *res = list_cons(set_copyElement(set_get_first(a)), *res);
    }
      
    else
      set_union(res, b, a);
  }
  else if (set_is_empty(a))
    set_copy(res, b);
  else
    set_copy(res, a);
}

/**
 * Erzeugt die Schnittmenge zweier Mengen a und b.
 *
 * @param[in,out] res Zeiger auf die Schnittmenge von a und b.
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @pre res zeigt auf existierende leere Menge.
 */
void set_intersection (Set * res, Set a, Set b)
{
  assert(set_is_empty(*res));
  
  if (!set_is_empty(a) && !(set_is_empty(b)))
  {
    if (!list_geElement(set_get_first(a), set_get_first(b)))
      set_intersection(res, list_tail(a), b);
      
    else if (list_eqElement(set_get_first(a), set_get_first(b)))
    {
      set_intersection(res, list_tail(a), list_tail(b));
      *res = list_cons(set_copyElement(set_get_first(a)), *res);
    }
    
    else
      set_intersection(res, a, list_tail(b));
  }
}

/**
 * Erzeugt die Differenzmenge zweier Mengen a und b.
 *
 * @param[in,out] res Zeiger auf die Differenzmenge von a und b.
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @pre res zeigt auf existierende leere Menge.
 */
void set_difference (Set * res, Set a, Set b)
{
  assert(set_is_empty(*res));
  
  if (!set_is_empty(a) && !(set_is_empty(b)))
  {
    if (!list_geElement(set_get_first(a), set_get_first(b)))
    {
      set_difference(res, list_tail(a), b);
      *res = list_cons(set_copyElement(set_get_first(a)), *res);
    }
    
    else if (list_eqElement(set_get_first(a), set_get_first(b)))
      set_difference(res, list_tail(a), list_tail(b));

    else
      set_difference(res, a, list_tail(b));
  }
  else if (set_is_empty(b))
  {
    set_copy(res, a);
  }
}

/**
 * Erzeugt die symmetrische Differenzmenge zweier Mengen a und b.
 *
 * @param[in,out] res Zeiger auf die symmetrische Differenzmenge von a und b.
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @pre res zeigt auf existierende leere Menge.
 */
void set_symmetric_difference (Set * res, Set a, Set b)
{
  assert(set_is_empty(*res));
  
  if (!set_is_empty(a) && !set_is_empty(b))
  {
    if (!list_geElement(set_get_first(a), set_get_first(b)))
    {
      set_symmetric_difference(res, list_tail(a), b);
      *res = list_cons(set_copyElement(set_get_first(a)), *res);
    }
    
    else if (list_eqElement(set_get_first(a), set_get_first(b)))
      set_symmetric_difference(res, list_tail(a), list_tail(b));
      
    else
      set_symmetric_difference(res, b, a);
  }
  else if (set_is_empty(a))
    set_copy(res, b);
  else
    set_copy(res, a);
}

/**
 * Erzeugt eine Kopie der Menge s.
 *
 * @param[in,out] res Zeiger auf die Kopie von s.
 * @param[in] s die Ausgangsmenge.
 *
 * @pre res zeigt auf existierende leere Menge.
 * @post die Menge, auf die res zeigt, ist gleich s.
 */
void set_copy (Set * res, Set s)
{
  assert(set_is_empty(*res));
  
  if (!set_is_empty(s))
  {
    set_copy(res, list_tail(s));
    *res = list_cons(set_copyElement(set_get_first(s)), *res);
  }
  
  assert(set_equals(*res, s));
}

 /**
 * Erzeugt die zweielementige Partition der Menge a, bei der die Summe aller
 * Elemente in der ersten Menge (b) moeglichst gleich der Summe aller Elemente
 * in der zweiten Menge (c) ist. Die Summe der Elemente in der ersten Menge (b)
 * ist dabei immer groesser gleich der Summe der Elemente in der zweiten Menge
 * (c). Der Wert jedes Elements in der ersten Menge (b) ist zudem kleiner als
 * der Wert jedes Elements in der zweiten Menge (c).
 *
 * @param[in,out] res1 Zeiger auf die erste Menge der Partition.
 * @param[in,out] res2 Zeiger auf die zweite Menge der Partition.
 * @param[in] s die Ausgangsmenge.
 *
 * @pre res1 zeigt auf existierende leere Menge.
 * @pre res2 zeigt auf existierende leere Menge.
 * @post die Schnittmenge der Menge, auf die res1 zeigt, und der
 *                  Menge, auf die res2 zeigt, ist die leere Menge.
 * @post die Vereinigungsmenge der Menge, auf die res1 zeigt, und der
 *                  Menge, auf die res2 zeigt, ist die Menge s.
 */
void set_partition (Set * res1, Set * res2, Set s)
{
#ifndef NDEBUG
  List test1 = list_mkEmpty();
  List test2 = list_mkEmpty();
#endif

  assert(set_is_empty(*res1));
  assert(set_is_empty(*res2));
  
  partition(res1, res2, s);
  
#ifndef NDEBUG 
  set_intersection(&test1, *res1, *res2);
  set_union(&test2, *res1, *res2);
#endif

  assert(set_is_empty(test1));
  assert(set_equals(test2, s));

#ifndef NDEBUG
  set_remove_all_elems(&test1);
  set_remove_all_elems(&test2);
#endif
}

/**
 * Gibt die Stringrepraesentation der Menge s auf stdout aus.
 *
 * @param[in] s die Menge.
 */
void set_print (Set s)
{
  fprintf(stdout, "(");
  
  if (!set_is_empty(s))
  {
    set_printElement(set_elementValue(set_get_first(s)));
    printSet(list_tail(s));
  }
  
  fprintf(stdout, ")\n");
}

