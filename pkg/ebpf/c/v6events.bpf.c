#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

struct bpf_map_def_pvt {
	__u32 type;
	__u32 key_size;
	__u32 value_size;
	__u32 max_entries;
	__u32 map_flags;
	__u32 pinning;
	__u32 inner_map_fd;
};

#define PIN_GLOBAL_NS	2

#define BPF_MAP_TYPE_RINGBUF 27

struct data_t {
	__u8   src_ip[16];
	__u32  src_port;
	__u8   dest_ip[16];
	__u32  dest_port;
	__u32  protocol;
	__u32  verdict;
};

struct conntrack_key {
	__u8  src_ip[16];
	__u16 src_port;
	__u8  dest_ip[16];
	__u16 dest_port;
	__u8  protocol;
};

struct conntrack_value {
	__u8 val[16];
};

struct bpf_map_def_pvt SEC("maps") aws_conntrack_map = {
    .type = BPF_MAP_TYPE_LRU_HASH,
    .key_size =sizeof(struct conntrack_key),
    .value_size = sizeof(struct conntrack_value),
    .max_entries = 65536,
    .pinning = PIN_GLOBAL_NS,
};

struct bpf_map_def_pvt SEC("maps") policy_events = {
    .type = BPF_MAP_TYPE_RINGBUF,
    .max_entries = 256 * 1024,
    .pinning = PIN_GLOBAL_NS,
};

char _license[] SEC("license") = "GPL";