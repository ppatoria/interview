       push   %rbp
       pxor   %xmm0,%xmm0
       mov    %rdi,%rbp
       push   %rbx
       sub    $0x28,%rsp
       mov    %rsp,%rdi
       movaps %xmm0,(%rsp)
       movq   $0x0,0x10(%rsp)
       callq  409a20 <void populateSimulatedInput<std::vector<MarketData, std::allocator<MarketData> > >(std::vector<MarketData, std::allocator<MarketData> >&)>
       cmpb   $0x0,0x1a(%rbp)
       je     408ff8 <ProcessByPrefetching(benchmark::State&)+0x58>
       mov    %rbp,%rdi
       callq  40bd70 <benchmark::State::StartKeepRunning()>
       mov    %rbp,%rdi
       callq  40bfa0 <benchmark::State::FinishKeepRunning()>
       mov    (%rsp),%rdi
       test   %rdi,%rdi
       je     4090af <ProcessByPrefetching(benchmark::State&)+0x10f>
       mov    0x10(%rsp),%rsi
       add    $0x28,%rsp
       pop    %rbx
       pop    %rbp
       sub    %rdi,%rsi
       jmpq   405480 <operator delete(void*, unsigned long)@plt>
       mov    %rbp,%rdi
       mov    0x10(%rbp),%rbx
       callq  40bd70 <benchmark::State::StartKeepRunning()>
       test   %rbx,%rbx
       je     408fd0 <ProcessByPrefetching(benchmark::State&)+0x30>
       nopl   0x0(%rax)
       mov    (%rsp),%rax
       mov    0x8(%rsp),%rsi
       cmp    %rax,%rsi
       je     408fd0 <ProcessByPrefetching(benchmark::State&)+0x30>
       mov    %rsi,%rdx
       sub    %rax,%rdx
       cmp    $0x600,%rdx
       ja     409090 <ProcessByPrefetching(benchmark::State&)+0xf0>
       nopl   (%rax)
       cmp    %rax,%rsi
       je     4090a0 <ProcessByPrefetching(benchmark::State&)+0x100>
       mov    %rsi,%rcx
0.03%  mov    %rcx,%rdi
0.03%  mov    %rax,%rdx
0.15%  sub    %rax,%rdi
       and    $0x8,%edi
       je     409058 <ProcessByPrefetching(benchmark::State&)+0xb8>
       lea    0x18(%rax),%rdx
       prefetcht0 (%rax)
       cmp    %rdx,%rcx
       je     409070 <ProcessByPrefetching(benchmark::State&)+0xd0>
       nopw   0x0(%rax,%rax,1)
0.03%  prefetcht0 (%rdx)
54.79% prefetcht0 0x18(%rdx)
40.68% add    $0x30,%rdx
0.06%  cmp    %rdx,%rcx
       jne    409058 <ProcessByPrefetching(benchmark::State&)+0xb8>
       nopl   0x0(%rax,%rax,1)
1.69%  add    $0x18,%rax
2.34%  cmp    %rax,%rcx
       jne    409070 <ProcessByPrefetching(benchmark::State&)+0xd0>
       cmp    %rcx,%rsi
       je     4090a0 <ProcessByPrefetching(benchmark::State&)+0x100>
0.03%  mov    %rcx,%rax
       mov    %rsi,%rdx
       sub    %rax,%rdx
       cmp    $0x600,%rdx
       jbe    409030 <ProcessByPrefetching(benchmark::State&)+0x90>
0.15%  lea    0x600(%rax),%rcx
0.03%  jmp    409038 <ProcessByPrefetching(benchmark::State&)+0x98>
       nopl   0x0(%rax)
       sub    $0x1,%rbx
       jne    409010 <ProcessByPrefetching(benchmark::State&)+0x70>
       jmpq   408fd0 <ProcessByPrefetching(benchmark::State&)+0x30>
       add    $0x28,%rsp
       pop    %rbx
       pop    %rbp
       retq
       mov    %rax,%rbx
       jmpq   405966 <ProcessByPrefetching(benchmark::State&) [clone .cold]>
