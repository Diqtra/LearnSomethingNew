#include <stdio.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int32.h>

// Callback function untuk menangani pesan yang diterima
void subscription_callback(const void * msgin) {
    if (msgin == NULL) return;
    const std_msgs__msg__Int32 *msg = (const std_msgs__msg__Int32 *)msgin;
    printf("[diqta_nih_bos]: Data diterima: %d\n", msg->data);
}

int main(int argc, char **argv) {
    // rcl_ret_t ret;
    rclc_support_t support;
    rcl_allocator_t allocator = rcl_get_default_allocator();
    rcl_node_t node;
    rcl_subscription_t subscriber;
    std_msgs__msg__Int32 msg;
    rclc_executor_t executor;

    // Inisialisasi ROS 2
    ret = rclc_support_init(&support, argc, argv, &allocator);
    // if (ret != RCL_RET_OK) {
    //     printf("[ERROR] Gagal inisialisasi support!\n");
    //     return -1;
    // }

    // Inisialisasi node
    ret = rclc_node_init_default(&node, "diqta_nih_bos", "", &support);
    // if (ret != RCL_RET_OK) {
    //     printf("[ERROR] Gagal membuat node!\n");
    //     return -1;
    // }

    // Inisialisasi subscriber
    ret = rclc_subscription_init_default(
        &subscriber, &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        "micro_ros_arduino_node_publisher"
    );
    // if (ret != RCL_RET_OK) {
    //     printf("[ERROR] Gagal membuat subscriber!\n");
    //     return -1;
    // }

    // Inisialisasi executor
    ret = rclc_executor_init(&executor, &support.context, 1, &allocator);
    // if (ret != RCL_RET_OK) {
    //     printf("[ERROR] Gagal inisialisasi executor!\n");
    //     return -1;
    // }

    rclc_executor_add_subscription(&executor, &subscriber, &msg, &subscription_callback, ON_NEW_DATA);

    // Loop utama
    while (rcl_context_is_valid(&support.context)) {
        rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
    }

    // Membersihkan resource
    rclc_executor_fini(&executor);
    // rcl_subscription_fini(&subscriber, &node);
    // rcl_node_fini(&node);
    rclc_support_fini(&support);

    return 0;
}
