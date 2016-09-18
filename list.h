/**
 * @file list.h Schnittstelle einer Bibliothek fuer Listenoperationen.
 *
 * Listenelemente sind ganzzahlige Werte groesser gleich Null.
 * Listen koennen jeden Wert nur einmal enthalten.
 * Listen sind aufsteigend sortiert.
 * Listen sind als rekursive Struktur implementiert (einfach verkette Listen).
 *
 * @author Martin Egge, Christian Uhlig, Uwe Schmidt
 */

#ifndef LIST_H__
#define LIST_H__ 1

/**
 * Prueft zwei Listenelemente auf Gleichheit.
 * Definiert zusammen mit list_geElement die Ordnungsrelation der Elemente
 *
 * @param[in] x erstes Element.
 * @param[in] y zweites Element.
 *
 * @return 1 wenn Elemente gleich sind, sonst 0.
 */
#define list_eqElement(x,y) ((x)==(y))

/**
 * Prueft, ob ein Listenelement groesser oder gleich einem zweiten ist.
 * Definiert zusammen mit list_eqElement die Ordnungsrelation der Elemente
 *
 * @param[in] x erstes Element.
 * @param[in] y zweites Element.
 *
 * @return 1 wenn x groesser oder gleich y ist, sonst 0.
 */
#define list_geElement(x,y) ((x)>=(y))

/** Datentyp der Listenelemente */
typedef unsigned long Element;

/** Datentyp der Liste. Hier verkette Liste. */
typedef struct Node *List;

/** Struktur eines Knotens der Liste. */
struct Node
{
  /** Zeiger auf nachfolgenden Knoten. */
  List next;
  /** Nutzlast des Knotens (Wert des Listenelementes), */
  Element value;
};

/**
 * Erzeugt eine leere Liste.
 *
 * @return leere Liste.
 */
extern List list_mkEmpty (void);

/**
 * Prueft, ob die Liste l leer ist.
 *
 * @param[in] l Liste, die geprüft werden soll.
 *
 * @return 1, wenn die Liste l leer ist, sonst 0.
 */
extern int list_isEmpty (List l);

/**
 * Liefert das erste Element der Liste l.
 *
 * @param[in] l Liste, deren erstes Element geliefert werden soll.
 *
 * @return erstes Element der Liste l.
 *
 * @pre Die Liste l darf nicht leer sein.
 */
extern Element list_head (List l);

/**
 * Liefert die Liste l ohne das erste Element.
 *
 * @param[in] l Liste, die ohne das erste Element geliefert werden soll.
 *
 * @return Liste l ohne das erste Element.
 *
 * @pre Die Liste l darf nicht leer sein.
 */
extern List list_tail (List l);

/**
 * Loescht das erste Element der Liste l.
 *
 * @param[in] l Liste, deren erstes Element geloescht werden soll.
 *
 * @return Liste, die durch Loeschen entsteht.
 *
 * @pre Die Liste l darf nicht leer sein.
 */
extern List list_removeHead (List l);

/**
 * Fuegt am Kopf der Liste l einen Knoten mit dem Wert e hinzu.
 *
 * @param[in] e Wert des Knotens, der hinzugefuegt werden soll.
 * @param[in] l Liste, an deren Kopf der Knoten hinzugefuegt werden soll.
 *
 * @return Liste, die durch Hinzufuegen entsteht.
 */
extern List list_cons (Element e, List l);

/**
 * Liefert die Laenge der Liste l.
 *
 * @param[in] l Liste, deren Laenge geliefert werden soll.
 *
 * @return Laenge der Liste l.
 */
extern unsigned long list_length (List l);

/**
 * Liefert das Element an der Position i in der Liste l.
 *
 * @param[in] l Liste, deren Element an der Position i geliefert werden soll.
 * @param[in] i Position des Elements, das geliefert werden soll.
 *
 * @return Element, das an der Position i in der Liste l steht.
 */
extern Element list_at (List l, unsigned long i);

/**
 * Prueft, ob das Element e in der Liste l enthalten ist.
 *
 * @param[in] e Element, dessen Vorkommen in der Liste l geprueft werden soll.
 * @param[in] l Liste, deren Elemente geprueft werden sollen.
 *
 * @return 1, wenn das Element e in der Liste l enthalten ist, sonst 0.
 */
extern int list_isIn (Element e, List l);

/**
 * Fuegt das Element e in die Liste l ein.
 *
 * @param[in] e Element, das in die  Liste l eingefuegt werden soll.
 * @param[in] l Liste, in die das Element e eingefuegt werden soll..
 *
 * @return Liste, die das Element e enthaelt.
 */
extern List list_insertElem (Element e, List l);

/**
 * Entfernt das Element e aus der Liste l.
 *
 * @param[in] e Element, das aus der Liste l entfernt werden soll.
 * @param[in] l Liste, aus der das Element e entfernt werden sollen.
 *
 * @return Liste, die keine Elemente enthaelt.
 */
extern List list_removeElem (Element e, List l);

/**
 * Entfernt alle Elemente aus der Liste l.
 *
 * @param[in] l Liste, aus der alle Elemente entfernt werden sollen.
 *
 * @return Liste, die keine Elemente enthaelt.
 */
extern List list_removeAllElems (List l);

#endif
