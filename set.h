#ifndef __SET_H__
#define __SET_H__

/**
 * @file set.h Schnittstelle einer Bibliothek fuer Mengenoperationen.
 *
 * Mengen werden intern durch sortierte verkettete Listen dargestellt.
 *
 * @author Martin Egge, Christian Uhlig
 */

#include "list.h"

/** Eine Menge, hier als sortierte verkettete Liste implementiert. */
typedef List Set;

/**
 * Liefert den Wert des Elements e
 *
 * @param[in] e Element, dessen Wert geliefert werden soll.
 *
 * @return Wert des Elements e.
 */
#define set_elementValue(e) (e)

/**
 * Gibt die Stringrepraesentation des Elements e auf stdout aus.
 *
 * @param[in] e Element, dessen Stringrepraesentation  ausgegeben werden soll.
 */
#define set_printElement(e) printf ("%lu", e)

/**
 * Liefert die Kopie des Elements e.
 *
 * @param e Element, dessen Kopie geliefert werden soll.
 *
 * @return Kopie des Elements e.
 */
#define set_copyElement(e) (e)

/**
 * Initialisiert eine Menge als leere Menge.
 *
 * @param[out] s Zeiger auf die initialisierte Menge.
 */
void set_make_empty (Set * s);

/**
 * Fuegt ein Element e in eine Menge s ein.
 *
 * @param[in,out] s Zeiger auf die Menge.
 * @param[in] e das einzufuegende Element.
 *
 * @pre s ist Zeiger auf existierende Menge.
 * @post die Menge, auf die s zeigt, enthält den Wert e.
 */
void set_insert (Set * s, Element e);

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
void set_remove (Set * s, Element e);

/**
 * Loescht alle Elemente aus einer Menge.
 *
 * @param[in,out] s Zeiger auf die Menge.
 *
 * @pre s ist Zeiger auf existierende Menge.
 * @post s ist Zeiger auf leere Menge.
 */
void set_remove_all_elems (Set * s);

/**
 * Prueft, ob eine Menge leer ist.
 *
 * @param[in] s die Menge.
 *
 * @return 1, falls Menge leer ist; 0 sonst.
 */
int set_is_empty (Set s);

/**
 * Liefert die Kardinalitaet einer Menge.
 *
 * @param[in] s die Menge.
 *
 * @return Kardinalitaet einer Menge.
 */
unsigned long set_cardinality (Set s);

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
Element set_get_first (Set s);

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
Element set_get_last (Set s);

/**
 * Prueft, ob ein Element in einer Menge enthalten ist.
 *
 * @param[in] s die Menge.
 * @param[in] e das zu pruefende Element.
 *
 * @return 1, falls Element enthalten ist; 0 sonst.
 */
int set_contains (Set s, Element e);

/**
 * Prueft, ob die Menge a Untermenge der Menge b ist.
 *
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @return 1, falls Menge a Untermenge von b ist; 0 sonst.
 */
int set_is_subset (Set a, Set b);

/**
 * Prueft, ob die Menge a gleich der Menge b ist.
 *
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @return 1, falls Menge a gleich Menge b ist; 0 sonst.
 */
int set_equals (Set a, Set b);

/**
 * Erzeugt die Vereinigungsmenge zweier Mengen a und b.
 *
 * @param[in,out] res Zeiger auf die Vereinigungsmenge von a und b.
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @pre res zeigt auf existierende leere Menge.
 */
void set_union (Set * res, Set a, Set b);

/**
 * Erzeugt die Schnittmenge zweier Mengen a und b.
 *
 * @param[in,out] res Zeiger auf die Schnittmenge von a und b.
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @pre res zeigt auf existierende leere Menge.
 */
void set_intersection (Set * res, Set a, Set b);

/**
 * Erzeugt die Differenzmenge zweier Mengen a und b.
 *
 * @param[in,out] res Zeiger auf die Differenzmenge von a und b.
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @pre res zeigt auf existierende leere Menge.
 */
void set_difference (Set * res, Set a, Set b);

/**
 * Erzeugt die symmetrische Differenzmenge zweier Mengen a und b.
 *
 * @param[in,out] res Zeiger auf die symmetrische Differenzmenge von a und b.
 * @param[in] a die erste Menge.
 * @param[in] b die zweite Menge.
 *
 * @pre res zeigt auf existierende leere Menge.
 */
void set_symmetric_difference (Set * res, Set a, Set b);

/**
 * Erzeugt eine Kopie der Menge s.
 *
 * @param[in,out] res Zeiger auf die Kopie von s.
 * @param[in] s die Ausgangsmenge.
 *
 * @pre res zeigt auf existierende leere Menge.
 * @post die Menge, auf die res zeigt, ist gleich s.
 */
void set_copy (Set * res, Set s);

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
void set_partition (Set * res1, Set * res2, Set s);

/**
 * Gibt die Stringrepraesentation der Menge s auf stdout aus.
 *
 * @param[in] s die Menge.
 */
void set_print (Set s);

#endif
