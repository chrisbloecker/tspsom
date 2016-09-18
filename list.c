/**
 * @file list.c Schnittstelle einer Bibliothek fuer Listenoperationen.
 *
 * Listenelemente sind ganzzahlige Werte groesser gleich Null.
 * Listen koennen jeden Wert nur einmal enthalten.
 * Listen sind aufsteigend sortiert.
 * Listen sind als rekursive Struktur implementiert (einfach verkette Listen).
 *
 * @author Christopher Blöcker
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "list.h"

/* ########################################################################## */

/**
 * Erzeugt eine leere Liste.
 *
 * @return leere Liste.
 */
extern List list_mkEmpty (void)
{  
  return (List)0;
}

/**
 * Prueft, ob die Liste l leer ist.
 *
 * @param[in] l Liste, die geprüft werden soll.
 *
 * @return 1, wenn die Liste l leer ist, sonst 0.
 */
extern int list_isEmpty (List l)
{  
  return l == (List)0;
}

/**
 * Liefert das erste Element der Liste l.
 *
 * @param[in] l Liste, deren erstes Element geliefert werden soll.
 *
 * @return erstes Element der Liste l.
 *
 * @pre Die Liste l darf nicht leer sein.
 */
extern Element list_head (List l)
{
  assert(!list_isEmpty(l));
  
  return l->value;
}

/**
 * Liefert die Liste l ohne das erste Element.
 *
 * @param[in] l Liste, die ohne das erste Element geliefert werden soll.
 *
 * @return Liste l ohne das erste Element.
 *
 * @pre Die Liste l darf nicht leer sein.
 */
extern List list_tail (List l)
{
  assert(!list_isEmpty(l));
  
  return l->next;
}

/**
 * Loescht das erste Element der Liste l.
 *
 * @param[in] l Liste, deren erstes Element geloescht werden soll.
 *
 * @return Liste, die durch Loeschen entsteht.
 *
 * @pre Die Liste l darf nicht leer sein.
 */
extern List list_removeHead (List l)
{
  List res = NULL;
  
  assert(!list_isEmpty(l));
  
  res = list_tail(l);
  
  free(l);

  return res;
}

/**
 * Fuegt am Kopf der Liste l einen Knoten mit dem Wert e hinzu.
 *
 * @param[in] e Wert des Knotens, der hinzugefuegt werden soll.
 * @param[in] l Liste, an deren Kopf der Knoten hinzugefuegt werden soll.
 *
 * @return Liste, die durch Hinzufuegen entsteht.
 */
extern List list_cons (Element e, List l)
{
  List res = NULL;
  res = calloc(sizeof(*l), 1);
  
  if (!res) exit(1);

  res->value = e;
  res->next = l;
  
  return res;
}

/**
 * Liefert die Laenge der Liste l.
 *
 * @param[in] l Liste, deren Laenge geliefert werden soll.
 *
 * @return Laenge der Liste l.
 */
extern unsigned long list_length (List l)
{
  return (list_isEmpty(l)) ? 0 : 1 + list_length(list_tail(l));
}

/**
 * Liefert das Element an der Position i in der Liste l.
 *
 * @param[in] l Liste, deren Element an der Position i geliefert werden soll.
 * @param[in] i Position des Elements, das geliefert werden soll.
 *
 * @return Element, das an der Position i in der Liste l steht.
 */
extern Element list_at (List l, unsigned long i)
{
  return (i == 0) ? list_head(l) : list_at(list_tail(l), i - 1);
}

/**
 * Prueft, ob das Element e in der Liste l enthalten ist.
 *
 * @param[in] e Element, dessen Vorkommen in der Liste l geprueft werden soll.
 * @param[in] l Liste, deren Elemente geprueft werden sollen.
 *
 * @return 1, wenn das Element e in der Liste l enthalten ist, sonst 0.
 */
extern int list_isIn (Element e, List l)
{
  return (list_isEmpty(l)) ? 0 : (list_eqElement(list_head(l), e)) ? 1 : list_isIn(e, list_tail(l));
}

/**
 * Fuegt das Element e in die Liste l ein.
 *
 * @param[in] e Element, das in die  Liste l eingefuegt werden soll.
 * @param[in] l Liste, in die das Element e eingefuegt werden soll..
 *
 * @return Liste, die das Element e enthaelt.
 */
extern List list_insertElem (Element e, List l)
{
  if (list_isEmpty(l)) return list_cons(e, l);
  if (list_eqElement(list_head(l), e)) return l;
  if (list_geElement(list_head(l), e)) return list_cons(e, l);
  l->next = list_insertElem(e, list_tail(l));
  return l;
}

/**
 * Entfernt das Element e aus der Liste l.
 *
 * @param[in] e Element, das aus der Liste l entfernt werden soll.
 * @param[in] l Liste, aus der das Element e entfernt werden sollen.
 *
 * @return Liste, die keine Elemente enthaelt.
 */
extern List list_removeElem (Element e, List l)
{
  if (list_isEmpty(l)) return l;
  if (list_geElement(list_head(l), e) && !list_eqElement(list_head(l), e)) return l;
  if (list_eqElement(list_head(l), e)) return list_removeHead(l);
  l->next = list_removeElem(e, list_tail(l));
  return l;
}

/**
 * Entfernt alle Elemente aus der Liste l.
 *
 * @param[in] l Liste, aus der alle Elemente entfernt werden sollen.
 *
 * @return Liste, die keine Elemente enthaelt.
 */
extern List list_removeAllElems (List l)
{
  if (list_isEmpty(l)) return l;
  return list_removeAllElems(list_removeHead(l));
}

