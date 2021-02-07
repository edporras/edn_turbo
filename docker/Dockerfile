FROM buildpack-deps:stretch
MAINTAINER github@digressed.net
ARG ruby_version

ENV LC_ALL C.UTF-8

USER root
RUN groupadd -r ned -g 1000 && \
    useradd -u 1000 -r -g ned -m -s /sbin/nologin -c "Docker image user" ned && \
    mkdir /home/ned/bin && \
    mkdir /home/ned/src && \
    chown -R ned:ned /home/ned

WORKDIR /home/ned/src

# Update Ubuntu Software repository && update deps
RUN apt-get update && \
     apt-get install -y \
     libicu-dev \
     libreadline-dev \
     ragel && \
     rm -rf /var/lib/apt/lists/*

USER ned

ENV PATH "/home/ned/.rbenv/bin:/home/ned/.rbenv/shims:$PATH"

# install rbenv
RUN \curl -sL https://github.com/rbenv/rbenv-installer/raw/master/bin/rbenv-installer | bash - && \
    eval "$(rbenv init -)"

RUN rm -f .ruby-version

# the specified ruby version
RUN rbenv install $ruby_version && \
    rbenv global $ruby_version

COPY --chown=ned:ned . /home/ned/src

ENTRYPOINT ["docker/entrypoint"]
