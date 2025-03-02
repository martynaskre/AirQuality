#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"
#include "mongoose.h"
#include <string>

#include "Network/Network.h"

#define TEST_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )
#define TEST_TASK_STACK_SIZE			(( configSTACK_DEPTH_TYPE ) 2048)

static struct mg_mgr mgr;
static const char *s_listening_address = "http://0.0.0.0:80";

void device_dashboard_fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        mg_http_reply(c, 200, "Content-Type: application/json\r\n",
                      "{%m:%m}\n", MG_ESC("status"), MG_ESC("success"));
    }
}

void main_task(__unused void *params) {
    std::string ssid = "Stetiskes";
    std::string password = "malaDiec42";

    Network::joinNetwork(ssid, password);

    printf("trying to start mongoose \n");

    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, s_listening_address, device_dashboard_fn, &mgr); // Web listener

    printf("starting to poll \n");

    while (true) {
        mg_mgr_poll(&mgr, 10);
    }

    Network::deinitializeDriver();
}

void ap_task(__unused void *params) {
    Network::createAccessPoint();

    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, s_listening_address, device_dashboard_fn, &mgr); // Web listener

    printf("starting to poll \n");

    while (true) {
        mg_mgr_poll(&mgr, 10);
    }

    Network::deinitializeDriver();
}

int main() {
    stdio_init_all();

    TaskHandle_t task;
    xTaskCreate(ap_task, "TestMainThread", TEST_TASK_STACK_SIZE, NULL, TEST_TASK_PRIORITY, &task);
    vTaskStartScheduler();

    return 0;
}
