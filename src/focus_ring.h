#ifndef FOCUSRING_H
#define FOCUSRING_H

#include <gdk/gdk.h>


/* A FocusRing is an scrollable list that wraps around to the beginning/end
 * when the selection is moved out-of-bounds.
 */
typedef struct FocusRing_ {
    /* Points to current selection */
    const GList *selected;
    /* Points to beginning of list */
    const GList *beginning;
    /* Points to end of list */
    const GList *end;

    /* Function to retrieve the target "inner value" from the GList's data */
    gchar *(*getter_function)(gconstpointer);
} FocusRing;

FocusRing *initialize_focus_ring(const GList *options, gchar *(*getter_function)(gconstpointer), const gchar *const label);
void destroy_focus_ring(FocusRing *ring);

gchar *focus_ring_next(FocusRing *ring);
gchar *focus_ring_prev(FocusRing *ring);
gconstpointer focus_ring_get_selected(FocusRing *ring);
gchar *focus_ring_get_value(FocusRing *ring);
gchar *focus_ring_scroll_to_value(FocusRing *ring, const gchar *target_value);

#endif
