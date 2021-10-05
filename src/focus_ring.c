/* Functions related to a ring of focusable items */

#include "focus_ring.h"

static gint find_by_value(gconstpointer data, gconstpointer user_data);


/* Initialize the FocusRing with the given options, value getter, & label
 *
 * Returns NULL if the passed GList is NULL.
 * Throws an error if cannot allocate memory for the FocusRing.
 */
FocusRing *initialize_focus_ring(const GList *options, gchar *(*getter_function)(gconstpointer), const gchar *const label)
{
    if (options == NULL) {
        return NULL;
    }

    FocusRing *ring = malloc(sizeof(FocusRing));
    if (ring == NULL) {
        g_error("Could not allocate memory for FocusRing: %s", label);
    }

    ring->selected = options;
    ring->beginning = options;
    ring->end = g_list_last((GList *) options);

    ring->getter_function = getter_function;

    return ring;
}

/* Free only the FocusRing & not the underlying GList */
void destroy_focus_ring(FocusRing *ring)
{
    free(ring);
}

/* Focus the next item in the ring, or loop back to the first item. */
gchar *focus_ring_next(FocusRing *ring)
{
    if (ring->selected->next == NULL) {
        ring->selected = ring->beginning;
    } else {
        ring->selected = ring->selected->next;
    }
    return focus_ring_get_value(ring);
}

/* Focus the previous item in the ring, or loop back to the last item. */
gchar *focus_ring_prev(FocusRing *ring)
{
    if (ring->selected->prev == NULL) {
        ring->selected = ring->end;
    } else {
        ring->selected = ring->selected->prev;
    }
    return focus_ring_get_value(ring);
}

/* Get the currently selected data. */
gconstpointer focus_ring_get_selected(FocusRing *ring)
{
    return ring->selected->data;
}

/* Get the inner value of the currently selected item. */
gchar *focus_ring_get_value(FocusRing *ring)
{
    return ring->getter_function(focus_ring_get_selected(ring));
}


/* Internal data used by the find_by_value & focus_ring_scroll_to_value
 * functions.
 */
struct FindByValueData {
    FocusRing *ring;
    const gchar *target_value;
};

/* Attempt to scroll the ring to the item with the matching inner value.
 *
 * Does nothing if the item cannot be found.
 */
gchar *focus_ring_scroll_to_value(FocusRing *ring, const gchar *target_value)
{
    struct FindByValueData user_data = { .ring = ring, .target_value = target_value };
    GList *find_result = g_list_find_custom((GList *) ring->beginning, (void *) &user_data, &find_by_value);
    if (find_result != NULL) {
        ring->selected = find_result;
    }
    return focus_ring_get_value(ring);
}

/* Determine if the given data's inner value matches what we're looking for.
 *
 * Used as callback to the g_list_find_custom function.
 */
static gint find_by_value(gconstpointer data, gconstpointer user_data)
{
    struct FindByValueData *local_user_data = (struct FindByValueData *) user_data;

    return g_strcmp0(local_user_data->ring->getter_function(data), local_user_data->target_value);
}
