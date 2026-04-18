#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zmk/keymap.h>
#include <zmk/event_manager.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/ble.h>

static void update_os_layers(uint8_t profile) {
    switch (profile) {
        case 1:
            // Mac: Layer1を有効化、Layer2を無効化
            zmk_keymap_layer_deactivate(2);
            zmk_keymap_layer_activate(1);
            break;
        case 2:
            // iOS: Layer2を有効化、Layer1を無効化
            zmk_keymap_layer_deactivate(1);
            zmk_keymap_layer_activate(2);
            break;
        // Profile 0, 3以降は何もしない
    }
}

static int os_layer_listener_cb(const zmk_event_t *eh) {
    const struct zmk_ble_active_profile_changed *ev =
        as_zmk_ble_active_profile_changed(eh);
    if (ev) {
        update_os_layers(ev->index);
    }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(os_layer_listener, os_layer_listener_cb);
ZMK_SUBSCRIPTION(os_layer_listener, zmk_ble_active_profile_changed);

static int behavior_os_layer_init(void) {
    update_os_layers(zmk_ble_active_profile_index());
    return 0;
}

SYS_INIT(behavior_os_layer_init, APPLICATION, 95);
