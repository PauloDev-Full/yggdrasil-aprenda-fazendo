#include "../include/mongoose.h"
#include "../include/handlers.h"

static Node* root_tree = NULL;

static void serialize_tree_to_json(Node* root, struct mg_iobuf *io){
    if (root == NULL) {
        mg_printf(io, "null");
        return;
    }
    mg_printf(io, "{ \"value\": %d, \"height\": %d, \"left\": ", root->value, root->height);
    serialize_tree_to_json(root->left, io);
    mg_printf(io, ", \"right\": ");
    serialize_tree_to_json(root->right, io);
    mg_printf(io, " }");
}

void event_handler(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        char value_str[20];
        int value;

        if (mg_http_match_uri(hm, "/insert") && mg_vcmp(&hm->method, "POST") == 0) {

            mg_http_get_var(&hm->query, "value", value_str, sizeof(value_str));
            value = atoi(value_str);

            root_tree = insert(root_tree, value);

            struct mg_iobuf io;
            mg_iobuf_init(&io, 0, 0);
            serialize_tree_to_json(root_tree, &io);
            mg_http_reply(c, 200, "Content-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n", "%.*s", (int) io.len, io.buf);
            mg_iobuf_free(&io);
        }

        else if (mg_http_match_uri(hm, "/delete") && mg_vcmp(&hm->method, "POST") == 0) {

            mg_http_get_var(&hm->query, "value", value_str, sizeof(value_str));
            value = atoi(value_str);

            root_tree = deleteNode(root_tree, value);

            struct mg_iobuf io;
            mg_iobuf_init(&io, 0, 0);
            serialize_tree_to_json(root_tree, &io);
            mg_http_reply(c, 200, "Content-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n", "%.*s", (int) io.len, io.buf);
            mg_iobuf_free(&io);
        }

        else if(mg_http_match_uri(hm, "/rotate") && mg_vcmp(&hm->method, "POST") == 0){
            char type_str [10];

            mg_http_get_var(&hm->query, "value", value_str, sizeof(value_str));
            mg_http_get_var(&hm->query, "type", type_str, sizeof(type_str));

            int value = atoi(value_str);
            int typeRotation = atoi(type_str);

            root_tree = rotateInSpecificValue(root_tree, value, typeRotation);

            struct mg_iobuf io;
            mg_iobuf_init(&io, 0, 0);
            serialize_tree_to_json(root_tree, &io);
            mg_http_reply(c, 200, "Content-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n", "%.*s", (int) io.len, io.buf);
            mg_iobuf_free(&io);
        }
         else if (mg_http_match_uri(hm, "/reset") && mg_vcmp(&hm->method, "POST") == 0) {
            root_tree = resetTree(root_tree);
            struct mg_iobuf io;
            mg_iobuf_init(&io, 0, 0);
            serialize_tree_to_json(root_tree, &io);
            mg_http_reply(c, 200, "Content-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n", "%.*s", (int) io.len, io.buf);
            mg_iobuf_free(&io);
        }
        else if (mg_http_match_uri(hm, "/tree") && mg_vcmp(&hm->method, "GET") == 0) {
            struct mg_iobuf io;
            mg_iobuf_init(&io, 0, 0);
            serialize_tree_to_json(root_tree, &io);
            mg_http_reply(c, 200, "Content-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n", "%.*s", (int) io.len, io.buf);
            mg_iobuf_free(&io);
        }
        else {
            mg_http_reply(c, 404, "", "Endpoint não encontrado.\n");
        }
    }
}

void initiate_server(void)
{
    struct mg_mgr mgr;
    struct mg_connection *c;

    mg_mgr_init(&mgr);
    c = mg_http_listen(&mgr, "http://localhost:8080", event_handler, NULL);
    if (c == NULL) {
        fprintf(stderr, "Failed to start server\n");
        exit(1);
    }

    while (1) {
        mg_mgr_poll(&mgr, 1000);
    }

    mg_mgr_free(&mgr);
}