FROM nginx:alpine

COPY nginx-prod.conf /etc/nginx/nginx-prod.conf
COPY nginx-dev.conf  /etc/nginx/nginx-dev.conf
COPY set-mode /usr/local/bin/set-mode

RUN chmod +x /usr/local/bin/set-mode && \
    ln -sf /etc/nginx/nginx-prod.conf /etc/nginx/nginx.conf
