def on_packet(src_ip, dest_ip, protocol, size):
    with open("netmon.log", "a") as f:
        f.write(f"{src_ip} -> {dest_ip} [{protocol}] {size} bytes\n")