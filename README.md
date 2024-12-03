# fix-server
1. Устанавливаем библиотеку QuickFIX
2. Запускаем make QUICKFIX_PATH=(путь до папки где лежит QuickFIX).
   По умолчанию путем для QuickFIX указана домашняя папка, если он лежит там - можно просто make.
3. Запускаем сервер bin/server server.cfg
4. Запускаем клиент bin/client client.cfg

P. S. В качестве клиента используем клиент из примеров самой QuickFIX
