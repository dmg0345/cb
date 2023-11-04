Circular Buffer Use Cases
========================================================================================================================

This sections cover some typical use cases for a circular buffer, for a description of the all functionality refer to
the *API* section. The examples below use `uint32_t`, but other types work the same way with the interface.

Note that for simplicity, there is no error handling in the examples shown.

#1: Writing, reading and other operations
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

.. code-block:: c

    #include <stdint.h>
    #include "cb/cb.h"

    // Circular buffer structure.
    cb_t cbuf;
    // Underlying linear buffer for the circular buffer, with required extra element.
    uint32_t lcbuf[10U + 1U];
    // Linear buffer where data read from the circular buffer will be written.
    uint32_t ldbuf[5U];
    // Linear buffer with data to write to the circular buffer.
    const uint32_t lsbuf[10U] = {0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U, 0x09U, 0x0AU};
    // Other variables.
    size_t filled = 0U, unfilled = 0U;
    bool is_full = false, is_empty = false;

    // Initialize circular buffer, without subscribing to any events.
    cb_init(&cbuf, lcbuf, 10U + 1U, sizeof(uint32_t), NULL, cb_evt_id_none, NULL);

    // Write five elements, and obtain five filled elements, five unfilled elements, not full and not empty.
    cb_write(&cbuf, lsbuf, 5U);
    cb_get_filled(&cbuf, &filled);
    cb_get_unfilled(&cbuf, &unfilled);
    cb_is_full(&cbuf, &is_full);
    cb_is_empty(&cbuf, &is_empty);

    // Write five elements, and obtain ten filled elements, zero unfilled elements, full and not empty.
    cb_write(&cbuf, lsbuf, 5U);
    cb_get_filled(&cbuf, &filled);
    cb_get_unfilled(&cbuf, &unfilled);
    cb_is_full(&cbuf, &is_full);
    cb_is_empty(&cbuf, &is_empty);

    // Read five elements, and obtain five filled elements, five unfilled elements, not full and not empty.
    cb_read(&cbuf, ldbuf, 5U);
    cb_get_filled(&cbuf, &filled);
    cb_get_unfilled(&cbuf, &unfilled);
    cb_is_full(&cbuf, &is_full);
    cb_is_empty(&cbuf, &is_empty);

    // Read five elements, and obtain zero filled elements, ten unfilled elements, not full and empty.
    cb_read(&cbuf, ldbuf, 5U);
    cb_get_filled(&cbuf, &filled);
    cb_get_unfilled(&cbuf, &unfilled);
    cb_is_full(&cbuf, &is_full);
    cb_is_empty(&cbuf, &is_empty);

    // Deinitialize circular buffer.
    cb_deinit(&cbuf);

#2: Using events
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

.. code-block:: c

    #include <stdint.h>
    #include "cb/cb.h"

    // Event handler for the circular buffer.
    cb_error_t cb_evt_handler(cb_evt_t * const evt)
    {
        switch (evt->id)
        {
            case cb_evt_id_read:
            {
                // User provided read function, the event provides all the details of the operation.
                memcpy(evt->data.read.buffer, evt->data.read.read_ptr, evt->data.read.bytes);
            }
            break;

            case cb_evt_id_write:
            {
                // User provided write function, the event provides all the details of the operation.
                memcpy(evt->data.write.write_ptr, evt->data.write.buffer, evt->data.write.bytes);
            }
            break;

            case cb_evt_id_lock:
            {
                // User provided locking mechanism, this event must return cb_error_ok.
            }
            break;

            case cb_evt_id_unlock:
            {
                // User provided unlocking mechanism, this event must return cb_error_ok.
            }
            break;
        }

        return cb_error_ok;
    }

    // Circular buffer structure.
    cb_t cbuf;
    // Underlying linear buffer for the circular buffer, with required extra element.
    uint32_t lcbuf[10U + 1U];
    // Linear buffer where data read from the circular buffer will be written.
    uint32_t ldbuf[5U];
    // Linear buffer with data to write to the circular buffer.
    const uint32_t lsbuf[10U] = {0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U, 0x09U, 0x0AU};

    // Initialize circular buffer, providing an event handler and subscribing to events.
    const cb_evt_id_t sub_evts = cb_evt_id_read | cb_evt_id_write | cb_evt_id_lock | cb_evt_id_unlock;
    cb_init(&cbuf, lcbuf, 10U + 1U, sizeof(uint32_t), cb_evt_handler, sub_evts, NULL);
    // Write five elements, this will trigger write and lock/unlock events.
    cb_write(&cbuf, lsbuf, 5U);
    // Read five elements, this will trigger read and lock/unlock events.
    cb_read(&cbuf, lsbuf, 5U);
    // Deinitialize circular buffer.
    cb_deinit(&cbuf);
