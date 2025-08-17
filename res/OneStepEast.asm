# init snake #
movi r0, 1   # 内存地址
movi r1, 4   # 赋值
sw r1, 0(r0) # 写入 x = 4
sw r1, 1(r0) # 写入 y = 4

# move right 1 #
lw r1, 0(r0)   # 取出 x
addi r1, r1, 1 # x = x + 1
sw r1, 0(r0)   # 储存 x = 5
