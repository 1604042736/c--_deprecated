from frame import Frame


class Environment:
    '''
    虚拟机环境
    '''

    def __init__(self):
        self.frames = [Frame()]
        self.curframe = self.frames[-1]

    def add_frame(self, frame):
        '''
        添加栈帧
        '''
        self.frames.append(frame)
        self.curframe = frame

    def pop_frame(self):
        self.frames.pop()
        self.curframe = self.frames[-1]

    def __str__(self):
        return f'Environment(framenum={len(self.frames)})'
