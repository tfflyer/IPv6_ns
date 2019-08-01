import operator
import json

class Date_Handle:
    src_add=[]
    int_add=[]
    hoplimit=0
    r_index=0

    def __init__(self,list):
        self.src_add=list[0:32]
        self.int_add=list[38:70]
        self.hoplimit=list[34:36]
        m=self.int_add[6:8]
        # print(m)
        self.r_index=int(m[0])*10+int(m[1])


def date_r(filename):
    list=[]
    f = open(filename, "r")
    line = f.readline()
    # 0——31：src , 34——35：hoplimit, 38-69:interface,
    while line:
        list.append(line)
        # print(line)
        line=f.readline()
    return list


if __name__=="__main__":
    date_list=date_r('ipv6_data.txt')
    # list中的每一个元素就是一个溯源包的信息
    st_list=[]
    # 对象列表
    in_list=[]
    router_list=[]
    for i in date_list:
        date_=Date_Handle(i)
        if date_.int_add in in_list:
            # print('重复')
            pass
        else:
            in_list.append(date_.int_add)
            st_list.append(date_)
    # 去除重复
    order=operator.attrgetter('hoplimit')
    st_list.sort(key=order)
    # 排序
    print('可能的攻击路径为')
    for i in st_list:
        if i.src_add == st_list[1].src_add:
            print('第'+str(i.r_index)+'子网')
            print('路由器接口地址为：'+str(i.int_add))
            router_list.append(i.r_index)
            print('距离本主机跳数：'+str(64-int(i.hoplimit)+1))
    for i in range(0,len(router_list)):
        if router_list[i] == 6:
            router_list[i]=4
        elif router_list[i] == 8:
            router_list[i]=5
        elif router_list[i]==9:
            router_list[i]=6
    print(router_list)
    meta_data=[{'category':1,'name': 'dst','value': 15,},
                {'category':0,'name': 'router6','value': 15,'symbolSize':30},
                {'category':0,'name': 'router7','value': 20,'symbolSize':30},
                {'category':0,'name': 'router5','value': 13,'symbolSize':30},
                {'category':0,'name': 'router8','value': 18,'symbolSize':30},
                {'category':0,'name': 'router4','value': 10,'symbolSize':30},
                {'category':0,'name': 'router2','value': 4, 'symbolSize': 30},
                {'category':0,'name': 'router3','value': 8,'symbolSize': 30},
                {'category':0,'name': 'router1','value': 3, 'symbolSize': 30,},
                {'category':1,'name': 'src','value': 0,}]
    for j in router_list:
        for i in meta_data:
            if i['name']=='router'+str(j):
                i['category']=2

    with open("./hmm.txt", 'w', encoding='utf-8') as json_file:
        json.dump(meta_data, json_file, ensure_ascii=False)

