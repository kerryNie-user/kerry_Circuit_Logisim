# init snake #
movi r0, 1   # 内存地址
movi r1, 4   # 赋值
sw r1, 0(r0) # 写入 x = 4
sw r1, 1(r0) # 写入 y = 4

# show snake #
lw r4, 0(r0) # 取出 x
lw r5, 1(r0) # 取出 y
movi r6, 1   # 清空 SD
movi r6, 4   # 把数据刷入 SD
movi r6, 2   # 在屏幕上刷新

MOVE_SNAKE:
# move right 1 #
lw r1, 0(r0)   # 取出 x
addi r1, r1, 1 # x = x + 1
sw r1, 0(r0)   # 储存 x = 5

# show snake #
lw r4, 0(r0) # 取出 x
lw r5, 1(r0) # 取出 y
movi r6, 1   # 清空 SD
movi r6, 4   # 把数据刷入 SD
movi r6, 2   # 在屏幕上刷新

jump MOVE_SNAKE