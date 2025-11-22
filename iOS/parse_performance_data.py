import json
from tabulate import tabulate

def parse_performance_data(json_str):
    # 完整阶段说明映射表
    stage_descriptions = {
        'navigationStart': ('页面导航起始', '基准时间点'),
        'fetchStart': ('开始获取资源', '缓存检查'),
        'domainLookupStart': ('DNS查询开始', '解析域名'),
        'domainLookupEnd': ('DNS查询完成', ''),
        'connectStart': ('TCP连接开始', '建立通信通道'),
        'connectEnd': ('TCP连接完成', ''),
        'requestStart': ('HTTP请求开始', '发送请求头'),
        'responseStart': ('接收首字节', 'TTFB时间'),
        'responseEnd': ('响应完成', '接收最后字节'),
        'domLoading': ('开始DOM解析', '解析HTML文档'),
        'domInteractive': ('DOM解析完成', '可交互状态'),
        'domContentLoadedEventStart': ('DOMContentLoaded开始', '事件触发'),
        'domContentLoadedEventEnd': ('DOMContentLoaded结束', '回调执行完成'),
        'domComplete': ('页面完全加载', '所有资源就绪'),
        'loadEventStart': ('load事件触发', '最终加载完成')
    }

    data = json.loads(json_str)
    
    # 过滤有效数据并转换
    timeline = []
    for stage, timestamp in data.items():
        if timestamp > 0:
            desc, detail = stage_descriptions.get(stage, (stage, '未知阶段'))
            timeline.append({
                "timestamp": timestamp,
                "stage": stage,
                "desc": desc,
                "detail": detail
            })
    
    # 按时间戳排序
    timeline.sort(key=lambda x: x['timestamp'])
    
    # 计算耗时指标（精确到毫秒）
    navigation_start = timeline[0]['timestamp'] if timeline else 0
    prev_time = navigation_start
    for entry in timeline:
        entry_duration = entry['timestamp'] - prev_time
        entry['duration'] = max(entry_duration, 0)  # 防止负值
        entry['cumulative'] = entry['timestamp'] - navigation_start
        prev_time = entry['timestamp']
    
    # 构建表格数据
    table_data = []
    for entry in timeline:
        # 将detail中的换行符替换为空格
        desc_text = entry['desc']
        if entry['detail']:
            desc_text += f" ({entry['detail']})"
        row = [
            entry['timestamp'],
            entry['stage'],
            desc_text,
            entry['duration'],
            entry['cumulative']
        ]
        table_data.append(row)
    
    headers = ["时间戳(ms)", "阶段", "说明", "阶段耗时(ms)", "累计耗时(ms)"]
    return tabulate(table_data, headers, tablefmt="github", stralign="left", numalign="right")

# 使用示例
input_data = """
{
    "navigationStart": 1744363831516,
    "unloadEventStart": 0,
    "unloadEventEnd": 0,
    "redirectStart": 0,
    "redirectEnd": 0,
    "fetchStart": 1744363831516,
    "domainLookupStart": 1744363831516,
    "domainLookupEnd": 1744363831516,
    "connectStart": 1744363831516,
    "connectEnd": 1744363831516,
    "secureConnectionStart": 0,
    "requestStart": 1744363831516,
    "responseStart": 1744363831516,
    "responseEnd": 1744363831517,
    "domLoading": 1744363831518,
    "domInteractive": 1744363831536,
    "domContentLoadedEventStart": 1744363831536,
    "domContentLoadedEventEnd": 1744363831537,
    "domComplete": 1744363831537,
    "loadEventStart": 1744363831537,
    "loadEventEnd": 0
}
"""

print(parse_performance_data(input_data))
