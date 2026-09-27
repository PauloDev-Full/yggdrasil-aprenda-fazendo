#include "../include/mongoose.h"
#include "../include/handlers.h"
#include <stdio.h>
#include <stdlib.h>

static Node* root_tree = NULL;

Node* resetTree(Node* root);

static int serialize_tree_to_json(Node* root, char *buf, int max_len) {
    if (root == NULL) {
        return snprintf(buf, max_len, "null");
    }
    
    char left_buf[2048] = {0};
    char right_buf[2048] = {0};
    
    serialize_tree_to_json(root->left, left_buf, sizeof(left_buf));
    serialize_tree_to_json(root->right, right_buf, sizeof(right_buf));
    
    return snprintf(buf, max_len, "{ \"value\": %d, \"height\": %d, \"left\": %s, \"right\": %s }", 
                    root->value, root->height, left_buf, right_buf);
}

void event_handler(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        char value_str[20] = {0};
        static char json_response[16384]; 
        memset(json_response, 0, sizeof(json_response));

        if (mg_match(hm->uri, mg_str("/insert"), NULL) && mg_strcmp(hm->method, mg_str("POST")) == 0) {
            mg_http_get_var(&hm->query, "value", value_str, sizeof(value_str));
            int value = atoi(value_str);
            root_tree = insert(root_tree, value);
            serialize_tree_to_json(root_tree, json_response, sizeof(json_response));
            mg_http_reply(c, 200, "Content-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n", "%s", json_response);
        }
        else if (mg_match(hm->uri, mg_str("/delete"), NULL) && mg_strcmp(hm->method, mg_str("POST")) == 0) {
            mg_http_get_var(&hm->query, "value", value_str, sizeof(value_str));
            int value = atoi(value_str);
            root_tree = deleteNode(root_tree, value);
            serialize_tree_to_json(root_tree, json_response, sizeof(json_response));
            mg_http_reply(c, 200, "Content-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n", "%s", json_response);
        }
        else if (mg_match(hm->uri, mg_str("/rotate"), NULL) && mg_strcmp(hm->method, mg_str("POST")) == 0) {
            char type_str[10] = {0};
            mg_http_get_var(&hm->query, "value", value_str, sizeof(value_str));
            mg_http_get_var(&hm->query, "type", type_str, sizeof(type_str));
            int value = atoi(value_str);
            int typeRotation = atoi(type_str);
            root_tree = rotateInSpecificValue(root_tree, value, typeRotation);
            serialize_tree_to_json(root_tree, json_response, sizeof(json_response));
            mg_http_reply(c, 200, "Content-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n", "%s", json_response);
        }
        else if (mg_match(hm->uri, mg_str("/reset"), NULL) && mg_strcmp(hm->method, mg_str("POST")) == 0) {
            root_tree = resetTree(root_tree);
            serialize_tree_to_json(root_tree, json_response, sizeof(json_response));
            mg_http_reply(c, 200, "Content-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n", "%s", json_response);
        }
        else if (mg_match(hm->uri, mg_str("/tree"), NULL) && mg_strcmp(hm->method, mg_str("GET")) == 0) {
            serialize_tree_to_json(root_tree, json_response, sizeof(json_response));
            mg_http_reply(c, 200, "Content-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n", "%s", json_response);
        }

         else if (mg_match(hm->uri, mg_str("/js/d3.min.js"), NULL)) {
            mg_http_reply(c, 200, "Content-Type: application/javascript\r\nAccess-Control-Allow-Origin: *\r\n", "");
        }

        else {
            mg_http_reply(c, 404, "", "Endpoint nao encontrado.\n");
        }
    }
}

void initiate_server(void) {
    struct mg_mgr mgr;
    struct mg_connection *c;

    mg_mgr_init(&mgr);
    c = mg_http_listen(&mgr, "0.0.0.0:8080", event_handler, NULL);
    if (c == NULL) {
        fprintf(stderr, "Failed to start server\n");
        exit(1);
    }

    while (1) {
        mg_mgr_poll(&mgr, 1000);
    }

    mg_mgr_free(&mgr);
}
